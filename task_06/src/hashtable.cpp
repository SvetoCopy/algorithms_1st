#include "hashtable.hpp"

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

HashTable::HashTable() { hash_table_.resize(1000); }

bool HashTable::Insert(const std::string& key, int value) {
  int index = std::hash<std::string>{}(key) % hash_table_.size();

  if (hash_table_[index] != nullptr) return false;
  hash_table_[index] = new LinkedList(key, value);
  size_++;
  return true;
}

void HashTable::InsertOrUpdate(const std::string& key, int value) {
  if (Insert(key, value) == true) return;
  int index = std::hash<std::string>{}(key) % hash_table_.size();
  LinkedList* current_list = hash_table_[index];

  if (current_list->key == key) {
    current_list->value = value;
    return;
  }
  while (current_list->next != nullptr) {
    if (current_list->key == key) {
      current_list->value = value;
      return;
    }
    current_list = current_list->next;
  }
  current_list->next = new LinkedList(key, value);

  while (current_list->prev != nullptr) {
    current_list = current_list->prev;
  }
  size_++;
}

void HashTable::Remove(const std::string& key) {
  int index = std::hash<std::string>{}(key) % hash_table_.size();
  if (hash_table_[index] == nullptr) {
    throw std::range_error("Error");
  }
  if (hash_table_[index]->next == nullptr) {
    hash_table_[index] = nullptr;
    size_--;
    return;
  }
  LinkedList* current_list = hash_table_[index];
  while (current_list->next != nullptr) {
    if (current_list->key == key) {
      current_list->prev->next = current_list->next;
      current_list->next->prev = current_list->prev;
      delete current_list;
      size_--;
      return;
    }
    current_list = current_list->next;
  }
}

int HashTable::Find(const std::string& key) const {
  int index = std::hash<std::string>{}(key) % hash_table_.size();
  LinkedList* current_list = hash_table_[index];
  if (current_list == nullptr) {
    throw std::range_error("Error");
  }
  if (current_list->next == nullptr) {
    if (current_list->key == key) {
      return current_list->value;
    }
  }
  while (current_list->next != nullptr) {
    if (current_list->key == key) {
      return current_list->value;
    }
    current_list = current_list->next;
  }
  throw std::range_error("Error");
}

size_t HashTable::Size() const { return size_; }