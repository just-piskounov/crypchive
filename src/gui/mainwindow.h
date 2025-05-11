#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTableWidget>
#include <QHeaderView>
#include <QTabWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QApplication>
#include <unordered_map>
#include <vector>
#include <string>

#include "../core/huffman/huffman_wrapper.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void saveFile();
    void compressData();
    void decompressData();
    // Removed showStatistics() declaration as it's not implemented
    void updateHuffmanTable();

private:
    // UI Elements
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    
    // Compression tab
    QWidget *compressTab;
    QTextEdit *inputTextEdit;
    QPushButton *openFileButton;
    QPushButton *compressButton;
    QPushButton *saveCompressedButton;
    QLabel *compressionStatsLabel;
    QProgressBar *compressionProgressBar;
    QTableWidget *huffmanCodesTable;

    // Decompression tab
    QWidget *decompressTab;
    QTextEdit *compressedTextEdit;
    QPushButton *openCompressedButton;
    QPushButton *decompressButton;
    QPushButton *saveDecompressedButton;
    QLabel *decompressionStatsLabel;
    QProgressBar *decompressionProgressBar;

    // Data
    std::vector<byte> originalData;
    std::string compressedBinaryString;
    std::vector<byte> compressedData;
    std::unordered_map<byte, std::string> huffmanCodes;
    std::string huffmanTableSerialized;
    std::vector<byte> decompressedData;

    // Helper methods
    void setupUI();
    void updateCompressionStats();
    void updateDecompressionStats();
    QString bytesToHexString(const std::vector<byte>& data, int maxChars = 100);
    void displayHuffmanCodes();
};

#endif // MAINWINDOW_H
