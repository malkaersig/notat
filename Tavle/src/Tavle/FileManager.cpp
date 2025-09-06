#include "FileManager.h"

void FileManager::UpdateCursorPos()
{
	cursorPos = 0;
	Node* pScannerNode = pStartNode->pNext;
	while (pScannerNode->nodeType == WCHAR_NODE)
	{
		++cursorPos;
	}
}

FileManager::FileManager(const wchar_t* filepath) :
	filepathString(filepath)
{
	pStartNode = new Node(START_NODE);


	fileStream.open(filepathString.c_str());
	try
	{
		if (!fileStream.is_open())
		{
			throw FileFailedToOpen("err: couldnt open file");
		}
		pCurrentNode = pStartNode;
		wchar_t c;

		while (fileStream.get(c))
		{
			pCurrentNode->pNext = new Node(c);
			pCurrentNode->pNext->pPrev = pCurrentNode;
			pCurrentNode = pCurrentNode->pNext;
			++cursorPos;
		}
		pEndNode = new Node(END_NODE);
		pEndNode->pPrev = pCurrentNode;
		pCurrentNode->pNext = pEndNode;
	}
	catch (FileFailedToOpen)
	{
		// idk return failed or smth
		// f close gets called in the destructor
	}
}

FileManager::~FileManager()
{
	if (fileStream.is_open())
		fileStream.close();
	delete pStartNode;
}

void FileManager::Insert(const wchar_t& c)
{
	Node* pInsertNode = new Node(c);
	pInsertNode->pPrev = pCurrentNode;
	pInsertNode->pNext = pCurrentNode->pNext;
	pCurrentNode->pNext = pInsertNode;
	pInsertNode->pNext->pPrev = pInsertNode;
	pCurrentNode = pInsertNode;
	++cursorPos;
}

void FileManager::Delete()
{
	if (pCurrentNode == pStartNode)
	{
		return;
	}
	Node* pDyingNode = pCurrentNode;
	pCurrentNode = pCurrentNode->pPrev;
	pDyingNode->pPrev->pNext = pDyingNode->pNext;
	pDyingNode->pNext->pPrev = pDyingNode->pPrev;
	pDyingNode->pNext = nullptr;
	pDyingNode->pPrev = nullptr;
	delete pDyingNode;
	--cursorPos;
}

void FileManager::Save()
{
	if (fileStream.is_open())
		fileStream.close();
	std::wofstream outFStream(filepathString);

	if (!outFStream)
	{
		std::cerr << "Failed to open file when saving!" << std::endl;
		return;
	}
	Node* pScannerNode = pStartNode->pNext;
	while (pScannerNode->nodeType == WCHAR_NODE)
	{
		outFStream.put(*static_cast<wchar_t*>(pScannerNode->pElement));
		pScannerNode = pScannerNode->pNext;
	}
	outFStream.close();
}

void FileManager::Next()
{
	if (pCurrentNode->pNext->nodeType == END_NODE)
	{
		return;
	}
	pCurrentNode = pCurrentNode->pNext;
	++cursorPos;
}

void FileManager::Prev()
{
	if (pCurrentNode->pPrev == nullptr)
	{
		return;
	}
	pCurrentNode = pCurrentNode->pPrev;
	--cursorPos;
}

void FileManager::GoTo(unsigned int newPos)
{
	if (newPos == cursorPos)
	{
		return;
	}
	if (newPos > cursorPos)
	{
		while (cursorPos != newPos)
		{
			++cursorPos;
			pCurrentNode = pCurrentNode->pNext;
		}
		return;
	}
	if (newPos < cursorPos)
	{
		while (cursorPos != newPos)
		{
			--cursorPos;
			pCurrentNode = pCurrentNode->pPrev;
		}
		return;
	}
}

std::wstring FileManager::Filepath()
{
	return filepathString;
}

TextDrawingInfo FileManager::CreateTextDrawingInfo()
{
	std::wstringstream ss;
	Node* pScannerNode = pStartNode->pNext;
	while (pScannerNode->nodeType == WCHAR_NODE)
	{
		ss << *static_cast<wchar_t*>(pScannerNode->pElement);
		pScannerNode = pScannerNode->pNext;
	}
	TextDrawingInfo drawingInfo = { ss.str(), cursorPos };
	return drawingInfo;
}

std::wstring FileManager::CreateSubstring(unsigned int startPos, unsigned int endPos)
{
	//startPos += 2;
	//endPos += 2;
	Node* pScannerNode = pStartNode;
	unsigned int scannerPos = 0;
	bool nodeTypeIsValid = (pScannerNode->nodeType == WCHAR_NODE);
	bool positionIsValid = (scannerPos == startPos);
	while (!nodeTypeIsValid || !positionIsValid)
	{
		
		if (pScannerNode->nodeType == WCHAR_NODE)
			++scannerPos;
		pScannerNode = pScannerNode->pNext;
		nodeTypeIsValid = (pScannerNode->nodeType == WCHAR_NODE);
		positionIsValid = (scannerPos == startPos);
	}
	

	std::wstringstream ss;

	//ss << L"CopyInfo([" << std::to_wstring(scannerPos) << L", " << *static_cast<wchar_t*>(pScannerNode->pElement) << "]," << std::to_wstring(startPos) << L", " << std::to_wstring(endPos) << L"), Selection(";

	//return ss.str();

	while (pScannerNode->nodeType == WCHAR_NODE && scannerPos < endPos)
	{

		ss << *static_cast<wchar_t*>(pScannerNode->pElement);
		//ss << std::to_wstring(scannerPos);
		pScannerNode = pScannerNode->pNext;
		++scannerPos;
	}
	//ss << ")";
	return ss.str();
}

inline Node::Node(NODE_DATA type) :
	nodeType(type),
	pElement(nullptr)
{}

inline Node::Node(const wchar_t& e) :
	pElement(new wchar_t(e)),
	nodeType(WCHAR_NODE)
{}

inline Node::~Node()
{
	if (nodeType == WCHAR_NODE)
	{
		delete pElement;
	}
	if (pNext)
	{
		delete pNext;
	}
}
