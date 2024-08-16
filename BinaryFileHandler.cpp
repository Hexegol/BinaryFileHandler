#include <iostream>
#include "BinaryFileHandler.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <string>

std::ofstream BinaryFileHandler::wOpenFile(const std::string& fileName)
{
    std::ofstream file;
    file.open(fileName, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "failed to open file : " << fileName << std::endl;
        throw std::ios_base::failure("failed to open file for writing");
    }

    return file;
}

void BinaryFileHandler::wWriteHeader(std::ofstream& file, const Header& header) {
    file.write(reinterpret_cast<const char*>(&header), sizeof(Header));
    if (!file) {
        throw std::ios_base::failure("Failed to write header to file");
    }
}

void BinaryFileHandler::wWriteRecords(std::ofstream& file, const std::vector<Data>& records)
{
    for (Data r : records)
    {
        file.write(reinterpret_cast<const char*>(&r), sizeof(r));
    }
}

std::ifstream BinaryFileHandler::rOpenFile(const std::string& fileName)
{
    std::ifstream file;
    if (!file.is_open())
    {
        std::cerr << "failed to open file : " << fileName << std::endl;
        throw std::ios_base::failure("failed to open file for writing");
    }
    return file;
}

Header BinaryFileHandler::rReadHeader(std::ifstream& file)
{
    if (!file.is_open()) {
        throw std::ios_base::failure("File is not open");
    }
    Header header;

    file.read(reinterpret_cast<char*>(&header), sizeof(Header));
    if (!file) {
        throw std::ios_base::failure("Failed to read header from file");
    }

    const char expectedSignature[4] = { 'O', 'M', 'E', 'G' };
    if (std::memcmp(header.signature, expectedSignature, 4) != 0) {
        throw std::ios_base::failure("Invalid file signature");
    }
    return header;
}

void BinaryFileHandler::rReadRecords(std::ifstream& file, std::vector<Data>& records) {
    for (auto& record : records) {
        file.read(reinterpret_cast<char*>(&record), sizeof(Data));
        if (!file) {
            throw std::ios_base::failure("Failed to read record from file");
        }
    }
}

void BinaryFileHandler::Write(const std::string& fileName, const std::vector<Data>& records) {
    auto file = wOpenFile(fileName); 

    Header header;
    strcpy_s(header.signature, sizeof(header.signature), "DATA");
    header.version = 1;
    header.records = static_cast<uint32_t>(records.size()); 
    wWriteHeader(file, header);
    wWriteRecords(file, records); 

    file.close();
    if (!file) {
        throw std::ios_base::failure("Failed to close the file");
    }
}

void BinaryFileHandler::Read(const std::string& fileName, std::vector<Data>& records) {
    std::ifstream file = rOpenFile(fileName);
    Header header = rReadHeader(file);
    if (std::strncmp(header.signature, "DATA", 4) != 0) {
        throw std::ios_base::failure("Invalid file signature");
    }
    records.resize(header.records);
    rReadRecords(file, records);
    file.close();
    if (!file) {
        throw std::ios_base::failure("Failed to close the file");
    }
}


int main() {
    std::string filename = "testfile.bin";

    //write
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "failed to open file for writing" << std::endl;
            return 1;
        }
        Header header = { "DATA", 1, 2 };
        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        Data record1 = { 1, "Alice" };
        Data record2 = { 2, "Bob" };
        file.write(reinterpret_cast<const char*>(&record1), sizeof(record1));
        file.write(reinterpret_cast<const char*>(&record2), sizeof(record2));

        file.close();
    }

    // read
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file for reading" << std::endl;
            return 1;
        }
        Header header;
        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        std::cout << "Signature : " << header.signature << std::endl;
        std::cout << "Version : " << header.version << std::endl;
        std::cout << "Record Count : " << header.records << std::endl;
        Data record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
            std::cout << "ID : " << record.id << ", Name : " << record.name << std::endl;
        }

        file.close();
    }

    return 0;
}
