#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <execution>
#include <memory>
#include <vector>


// Custom exception class
class FileFailedToOpen : public std::exception {
private:
	std::string message; // Message to describe the exception

public:
	// Constructor
	FileFailedToOpen(const std::string& msg) : message(msg) {}

	// Override what() method
	virtual const char* what() const noexcept override
	{
		return message.c_str(); // Return the error message
	}
};

enum NODE_DATA
{
	WCHAR_NODE,
	START_NODE,
	END_NODE
};

// omnidirectional linked list
struct Node
{
public:
	NODE_DATA nodeType;
	void* pElement;
	Node* pPrev = nullptr;
	Node* pNext = nullptr;

public:
	Node(NODE_DATA type);
	Node(const wchar_t& e);
	~Node();

};

struct TextDrawingInfo
{
	std::wstring string;
	unsigned int cursorPos;
};

class FileManager
{
private:
	std::wifstream fileStream;
	Node* pStartNode	= nullptr;
	Node* pCurrentNode	= nullptr;
	Node* pEndNode		= nullptr;
	std::wstring filepathString;
	// slow, only use if uncertain, runs through entire list to figure out where current node is and update cursorPos
	void UpdateCursorPos();
	unsigned int cursorPos = 0;

public:
	FileManager(const wchar_t* filepath);
	~FileManager();
	void Insert(const wchar_t& c);
	void Delete();
	void Save();
	void Next();
	void Prev();
	void GoTo(unsigned int newPos);
	std::wstring Filepath();
	TextDrawingInfo CreateTextDrawingInfo();
	std::wstring CreateSubstring(unsigned int startPos, unsigned int endPos);

};
