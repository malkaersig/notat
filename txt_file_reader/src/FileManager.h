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

// ambivalent linked list
template<typename T>
struct Node
{
public:
	Node(const T& e) :
		element(std::make_unique<T>(e))
	{}
	std::unique_ptr<T> element;
	std::shared_ptr<Node<T>> previous = nullptr;
	std::shared_ptr<Node<T>> next = nullptr;
};

template<class ITERABLE_TYPE, class ELEMENT_TYPE>
std::shared_ptr<Node<ELEMENT_TYPE>> create_linked_list(const ITERABLE_TYPE& iterable)
{
	auto first_node = std::make_shared<Node<ELEMENT_TYPE>>(*iterable.cbegin());
	auto previous = first_node;
	for (auto it = ++iterable.cbegin(); it != iterable.cend(); ++it)
	{
		auto node = std::make_shared<Node<ELEMENT_TYPE>>(*it);
		node->previous = previous;
		previous->next = node;
		previous = node;
	}
	return first_node;
}



template<class T>
std::shared_ptr<Node<T>> get_last_node(std::shared_ptr<Node<T>> node)
{
	while (node->next)
	{
		node = node->next;
	}
	return node;
}

template<typename T>
class Cursor
{
private:
	std::shared_ptr<Node<T>> node;

public:
	Cursor() :
		node(nullptr)
	{}
	Cursor(const T& e) :
		node(std::make_shared<Node<T>>(e))
	{}
	Cursor(std::shared_ptr<T> node) :
		node(node)
	{}
	std::shared_ptr<Node<T>> Get()
	{
		return node;
	}
	std::shared_ptr<Node<T>> GoTo(std::shared_ptr<Node<T>> new_node)
	{
		node = new_node;
		return node;
	}
	std::shared_ptr<Node<T>> Next()
	{
		node = node->next;
		return node;
	}
	std::shared_ptr<Node<T>> Previous()
	{
		node = node->previous;
		return node;
	}

	std::shared_ptr<Node<T>> Insert(const T& element)
	{
		auto new_node = std::make_shared<Node<T>>(element);
		if (node->next)
		{
			node->next->previous = new_node;
		}
		node->next = new_node;
		node = new_node;
		return node;
	}
	std::shared_ptr<Node<T>> Insert(std::shared_ptr<Node<T>> first_node)
	{
		auto last_node = get_last_node<T>(first_node);
		if (node->next)
		{
			last_node->next = node->next;
			node->next->previous = last_node;
		}
		node->next = first_node;
		return first_node;

	}
	std::shared_ptr<Node<T>> RemoveCurrent()
	{
		if (node->next && node->previous)
		{
			node->next->previous = node->previous;
			node->previous->next = node->next;
			node = node->next;
		}
		else if (node->previous)
		{
			node->previous->next = nullptr;
			node = node->previous;
		}
		else if (node->next)
		{
			node->next->previous = nullptr;
			node = node->next;
		}
		else
		{
			node = nullptr;
		}
		return node;
	}

};


class FileManager
{
private:
	Cursor<wchar_t> cursor_a;
	Cursor<wchar_t> cursor_b;
public:
	void LoadFile(const wchar_t* filepath);

	std::wstring GetString();

};
