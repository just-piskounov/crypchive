#include "mainwindow.h"
#include <QDebug>
#include <algorithm>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    
    // Connect signals and slots
    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(compressButton, &QPushButton::clicked, this, &MainWindow::compressData);
    connect(saveCompressedButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(openCompressedButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(decompressButton, &QPushButton::clicked, this, &MainWindow::decompressData);
    connect(saveDecompressedButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    
    // Initialize
    saveCompressedButton->setEnabled(false);
    compressButton->setEnabled(false);
    decompressButton->setEnabled(false);
    saveDecompressedButton->setEnabled(false);
    
    setWindowTitle("Huffman Compression Tool");
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create tab widget
    tabWidget = new QTabWidget(centralWidget);
    
    // Create compression tab
    compressTab = new QWidget();
    QVBoxLayout *compressLayout = new QVBoxLayout(compressTab);
    
    // Input section
    QGroupBox *inputGroupBox = new QGroupBox("Input Data");
    QVBoxLayout *inputLayout = new QVBoxLayout(inputGroupBox);
    
    inputTextEdit = new QTextEdit();
    inputTextEdit->setPlaceholderText("Enter text to compress or load from file...");
    inputLayout->addWidget(inputTextEdit);
    
    QHBoxLayout *inputButtonLayout = new QHBoxLayout();
    openFileButton = new QPushButton("Open File");
    compressButton = new QPushButton("Compress");
    inputButtonLayout->addWidget(openFileButton);
    inputButtonLayout->addWidget(compressButton);
    inputLayout->addLayout(inputButtonLayout);
    
    compressLayout->addWidget(inputGroupBox);
    
    // Output section
    QGroupBox *outputGroupBox = new QGroupBox("Compressed Output");
    QVBoxLayout *outputLayout = new QVBoxLayout(outputGroupBox);
    
    compressionStatsLabel = new QLabel("Compression Statistics:");
    outputLayout->addWidget(compressionStatsLabel);
    
    compressionProgressBar = new QProgressBar();
    compressionProgressBar->setValue(0);
    outputLayout->addWidget(compressionProgressBar);
    
    saveCompressedButton = new QPushButton("Save Compressed File");
    outputLayout->addWidget(saveCompressedButton);
    
    compressLayout->addWidget(outputGroupBox);
    
    // Huffman table section
    QGroupBox *huffmanGroupBox = new QGroupBox("Huffman Codes");
    QVBoxLayout *huffmanLayout = new QVBoxLayout(huffmanGroupBox);
    
    huffmanCodesTable = new QTableWidget(0, 3);
    huffmanCodesTable->setHorizontalHeaderLabels({"Symbol", "ASCII", "Huffman Code"});
    huffmanCodesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    huffmanLayout->addWidget(huffmanCodesTable);
    
    compressLayout->addWidget(huffmanGroupBox);
    
    // Add compression tab
    tabWidget->addTab(compressTab, "Compression");
    
    // Create decompression tab
    decompressTab = new QWidget();
    QVBoxLayout *decompressLayout = new QVBoxLayout(decompressTab);
    
    // Compressed input section
    QGroupBox *compressedInputGroupBox = new QGroupBox("Compressed Input");
    QVBoxLayout *compressedInputLayout = new QVBoxLayout(compressedInputGroupBox);
    
    compressedTextEdit = new QTextEdit();
    compressedTextEdit->setPlaceholderText("Load compressed file...");
    compressedTextEdit->setReadOnly(true);
    compressedInputLayout->addWidget(compressedTextEdit);
    
    QHBoxLayout *compressedButtonLayout = new QHBoxLayout();
    openCompressedButton = new QPushButton("Open Compressed File");
    decompressButton = new QPushButton("Decompress");
    compressedButtonLayout->addWidget(openCompressedButton);
    compressedButtonLayout->addWidget(decompressButton);
    compressedInputLayout->addLayout(compressedButtonLayout);
    
    decompressLayout->addWidget(compressedInputGroupBox);
    
    // Decompressed output section
    QGroupBox *decompressedOutputGroupBox = new QGroupBox("Decompressed Output");
    QVBoxLayout *decompressedOutputLayout = new QVBoxLayout(decompressedOutputGroupBox);
    
    decompressionStatsLabel = new QLabel("Decompression Statistics:");
    decompressedOutputLayout->addWidget(decompressionStatsLabel);
    
    decompressionProgressBar = new QProgressBar();
    decompressionProgressBar->setValue(0);
    decompressedOutputLayout->addWidget(decompressionProgressBar);
    
    saveDecompressedButton = new QPushButton("Save Decompressed File");
    decompressedOutputLayout->addWidget(saveDecompressedButton);
    
    decompressLayout->addWidget(decompressedOutputGroupBox);
    
    // Add decompression tab
    tabWidget->addTab(decompressTab, "Decompression");
    
    // Set main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(tabWidget);
    
    // Set window properties
    resize(800, 600);
}

void MainWindow::openFile() {
    // Determine if we're opening for compression or decompression
    bool isCompression = (sender() == openFileButton);
    
    QString fileName = QFileDialog::getOpenFileName(
        this,
        isCompression ? "Open File to Compress" : "Open Compressed File",
        QString(),
        isCompression ? "All Files (*)" : "Compressed Files (*.huff *.bin);;All Files (*)"
    );
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    if (isCompression) {
        // Convert to vector<byte> for compression
        originalData.clear();
        originalData.reserve(data.size());
        for (char c : data) {
            originalData.push_back(static_cast<byte>(c));
        }
        
        // Display in text edit
        inputTextEdit->setText(QString::fromUtf8(data));
        compressButton->setEnabled(!originalData.empty());
        
        // Clear any previous compression results
        saveCompressedButton->setEnabled(false);
        huffmanCodesTable->setRowCount(0);
        compressionStatsLabel->setText("Compression Statistics:");
        compressionProgressBar->setValue(0);
    } else {
        // Handle compressed file opening
        // First bytes are the serialized Huffman table
        // Followed by compressed data
        
        // Check if this is a valid compressed file
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);
        
        // Try to extract the table size
        file.open(QIODevice::ReadOnly);
        QByteArray tableData = file.readLine();
        file.close();
        
        if (tableData.isEmpty()) {
            QMessageBox::warning(this, "Error", "Invalid compressed file format.");
            return;
        }
        
        // Display hex representation in text edit
        compressedTextEdit->setText(bytesToHexString(std::vector<byte>(data.begin(), data.end())));
        
        // Store the compressed data
        compressedData.clear();
        compressedData.insert(compressedData.end(), data.begin(), data.end());
        
        // Extract header and compressed data
        std::string fileContent(data.begin(), data.end());
        size_t dataStart = fileContent.find("\n\n");
        
        if (dataStart == std::string::npos) {
            QMessageBox::warning(this, "Error", "Invalid compressed file format.");
            return;
        }
        
        // Extract Huffman table
        huffmanTableSerialized = fileContent.substr(0, dataStart);
        
        // Extract compressed data binary string
        compressedBinaryString = fileContent.substr(dataStart + 2);
        
        // Deserialize Huffman table
        huffmanCodes = deserializeHuffmanTable(huffmanTableSerialized);
        
        // Update UI
        decompressButton->setEnabled(true);
        updateHuffmanTable();
    }
}

void MainWindow::saveFile() {
    // Determine if we're saving compressed or decompressed data
    bool isCompressed = (sender() == saveCompressedButton);
    
    QString fileName = QFileDialog::getSaveFileName(
        this,
        isCompressed ? "Save Compressed File" : "Save Decompressed File",
        QString(),
        isCompressed ? "Compressed Files (*.huff *.bin)" : "All Files (*)"
    );
    
    if (fileName.isEmpty()) return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot write file: " + file.errorString());
        return;
    }
    
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);
    
    if (isCompressed) {
        // Save compressed data with Huffman table
        std::string fileContent = huffmanTableSerialized + "\n\n" + compressedBinaryString;
        file.write(fileContent.c_str(), fileContent.size());
    } else {
        // Save decompressed data
        file.write(reinterpret_cast<const char*>(decompressedData.data()), decompressedData.size());
    }
    
    file.close();
    QMessageBox::information(this, "Success", "File saved successfully!");
}

void MainWindow::compressData() {
    // Check if we have text in the input
    if (originalData.empty()) {
        // Get text from text edit
        QByteArray text = inputTextEdit->toPlainText().toUtf8();
        if (text.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter text or load a file to compress.");
            return;
        }
        
        // Convert to vector<byte>
        originalData.clear();
        originalData.reserve(text.size());
        for (char c : text) {
            originalData.push_back(static_cast<byte>(c));
        }
    }
    
    // Progress indicator
    compressionProgressBar->setValue(10);
    QApplication::processEvents();
    
    // Generate Huffman codes
    huffmanCodes = getHuffmanCodes(originalData);
    
    compressionProgressBar->setValue(40);
    QApplication::processEvents();
    
    // Encode data
    compressedBinaryString = encodeData(originalData, huffmanCodes);
    
    compressionProgressBar->setValue(70);
    QApplication::processEvents();
    
    // Convert to byte array
    compressedData = binaryStringToBytes(compressedBinaryString);
    
    // Serialize Huffman table
    huffmanTableSerialized = serializeHuffmanTable(huffmanCodes);
    
    compressionProgressBar->setValue(100);
    QApplication::processEvents();
    
    // Update UI
    updateCompressionStats();
    updateHuffmanTable();
    saveCompressedButton->setEnabled(true);
}

void MainWindow::decompressData() {
    if (compressedBinaryString.empty() || huffmanCodes.empty()) {
        QMessageBox::warning(this, "Error", "No valid compressed data to decompress.");
        return;
    }
    
    // Progress indicator
    decompressionProgressBar->setValue(25);
    QApplication::processEvents();
    
    // Decode data
    decompressedData = decodeData(compressedBinaryString, huffmanCodes);
    
    decompressionProgressBar->setValue(100);
    QApplication::processEvents();
    
    // Update UI
    updateDecompressionStats();
    saveDecompressedButton->setEnabled(true);
}

void MainWindow::updateCompressionStats() {
    if (originalData.empty() || compressedData.empty()) {
        return;
    }
    
    double originalSize = originalData.size();
    double compressedSize = compressedData.size();
    double ratio = (1.0 - (compressedSize / originalSize)) * 100.0;
    
    QString stats = QString("Original size: %1 bytes\n"
                          "Compressed size: %2 bytes\n"
                          "Compression ratio: %3%")
                          .arg(originalSize)
                          .arg(compressedSize)
                          .arg(ratio, 0, 'f', 2);
    
    compressionStatsLabel->setText(stats);
}

void MainWindow::updateDecompressionStats() {
    if (compressedData.empty() || decompressedData.empty()) {
        return;
    }
    
    double compressedSize = compressedData.size();
    double decompressedSize = decompressedData.size();
    
    QString stats = QString("Compressed size: %1 bytes\n"
                          "Decompressed size: %2 bytes\n"
                          "Original text recovered: %3")
                          .arg(compressedSize)
                          .arg(decompressedSize)
                          .arg(QString::fromUtf8(reinterpret_cast<const char*>(decompressedData.data()), 
                                                decompressedData.size()).left(100) + 
                              (decompressedData.size() > 100 ? "..." : ""));
    
    decompressionStatsLabel->setText(stats);
}

void MainWindow::updateHuffmanTable() {
    // Clear existing rows
    huffmanCodesTable->setRowCount(0);
    
    // Add a row for each Huffman code
    int row = 0;
    for (const auto& pair : huffmanCodes) {
        huffmanCodesTable->insertRow(row);
        
        // Symbol column (displayable version)
        QString symbol;
        if (pair.first >= 32 && pair.first <= 126) { // Printable ASCII
            symbol = QChar(pair.first);
        } else { // Non-printable
            symbol = QString("\\x%1").arg(static_cast<int>(pair.first), 2, 16, QChar('0'));
        }
        
        QTableWidgetItem* symbolItem = new QTableWidgetItem(symbol);
        huffmanCodesTable->setItem(row, 0, symbolItem);
        
        // ASCII value column
        QTableWidgetItem* asciiItem = new QTableWidgetItem(QString::number(static_cast<int>(pair.first)));
        huffmanCodesTable->setItem(row, 1, asciiItem);
        
        // Huffman code column
        QTableWidgetItem* codeItem = new QTableWidgetItem(QString::fromStdString(pair.second));
        huffmanCodesTable->setItem(row, 2, codeItem);
        
        row++;
    }
    
    // Sort by frequency (implicitly by code length)
    huffmanCodesTable->sortItems(2, Qt::AscendingOrder);
}

// Also need to implement displayHuffmanCodes() since it's in the header
void MainWindow::displayHuffmanCodes() {
    // This can just call updateHuffmanTable which already does this work
    updateHuffmanTable();
}

QString MainWindow::bytesToHexString(const std::vector<byte>& data, int maxChars) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    int count = 0;
    for (byte b : data) {
        ss << std::setw(2) << static_cast<int>(b) << " ";
        count += 3;
        
        if (count >= maxChars) {
            ss << "...";
            break;
        }
    }
    
    return QString::fromStdString(ss.str());
}
