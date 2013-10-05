#include "node.h"

Node::Node()
{
}

int Node::getLevel() {
    return this->level;
}

QString Node::getName() {
   return this->name;
}

QString Node::getValue() {
    return this->value;
}

bool Node::isUsed() {
    return this->used;
}

void Node::setLevel(int level) {
    this->level = level;
}

void Node::setName(QString name) {
    this->name = name;
}

void Node::setValue(QString value) {
    this->value = value;
}

void Node::setUsed(bool used) {
    this->used = used;
}
