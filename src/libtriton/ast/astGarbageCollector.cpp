//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the BSD License.
*/

#include <triton/astGarbageCollector.hpp>
#include <triton/exceptions.hpp>



namespace triton {
  namespace ast {

    AstGarbageCollector::AstGarbageCollector(bool isBackup)
    {
      this->backupFlag = isBackup;
    }


    AstGarbageCollector::AstGarbageCollector(const AstGarbageCollector& other)
    {
      this->copy(other);
    }


    void AstGarbageCollector::operator=(const AstGarbageCollector& other) {
      this->copy(other);
    }


    void AstGarbageCollector::copy(const AstGarbageCollector& other) {
      /* Remove unused nodes before the assignation */
      for (auto it = this->allocatedNodes.begin(); it != this->allocatedNodes.end(); it++) {
        if (other.allocatedNodes.find(*it) == other.allocatedNodes.end())
          delete *it;
      }
      this->allocatedNodes  = other.allocatedNodes;
      this->backupFlag      = true;
      this->variableNodes   = other.variableNodes;
    }


    AstGarbageCollector::~AstGarbageCollector() {
      if (this->backupFlag == false)
        this->freeAllAstNodes();
    }


    void AstGarbageCollector::freeAllAstNodes(void) {
      for (auto it = this->allocatedNodes.begin(); it != this->allocatedNodes.end(); it++)
        delete *it;

      this->variableNodes.clear();
      this->allocatedNodes.clear();
    }


    void AstGarbageCollector::freeAstNodes(std::set<triton::ast::AbstractNode*>& nodes) {
      std::set<triton::ast::AbstractNode*>::iterator it;

      for (it = nodes.begin(); it != nodes.end(); it++) {
        /* Remove the node from the global set */
        this->allocatedNodes.erase(*it);

        /* Remove the node from the global variables map */
        if ((*it)->getKind() == triton::ast::VARIABLE_NODE)
          this->variableNodes.erase(reinterpret_cast<triton::ast::VariableNode*>(*it)->getVarName());

        /* Delete the node */
        delete *it;
      }

      nodes.clear();
    }


    void AstGarbageCollector::extractUniqueAstNodes(std::set<triton::ast::AbstractNode*>& uniqueNodes, triton::ast::AbstractNode* root) const {
      if (root == nullptr)
        return;

      uniqueNodes.insert(root);
      for (auto it = root->getChildren().cbegin(); it != root->getChildren().cend(); it++)
        this->extractUniqueAstNodes(uniqueNodes, *it);
    }


    triton::ast::AbstractNode* AstGarbageCollector::recordAstNode(triton::ast::AbstractNode* node) {
      /* Record the node */
      this->allocatedNodes.insert(node);
      return node;
    }


    void AstGarbageCollector::recordVariableAstNode(const std::string& name, triton::ast::AbstractNode* node) {
      if(!this->variableNodes.emplace(name, node).second) {
        throw triton::exceptions::Ast("Can't register this variable as it already exists");
      }
    }


    const std::set<triton::ast::AbstractNode*>& AstGarbageCollector::getAllocatedAstNodes(void) const {
      return this->allocatedNodes;
    }


    const std::map<std::string, triton::ast::AbstractNode*>& AstGarbageCollector::getAstVariableNodes(void) const {
      return this->variableNodes;
    }


    triton::ast::AbstractNode* AstGarbageCollector::getAstVariableNode(const std::string& name) const {
      auto it = this->variableNodes.find(name);
      if (it != this->variableNodes.end())
        return it->second;
      return nullptr;
    }


    void AstGarbageCollector::setAllocatedAstNodes(const std::set<triton::ast::AbstractNode*>& nodes) {
      /* Remove unused nodes before the assignation */
      for (auto it = this->allocatedNodes.begin(); it != this->allocatedNodes.end(); it++) {
        if (nodes.find(*it) == nodes.end())
          delete *it;
      }
      this->allocatedNodes = nodes;
    }


    void AstGarbageCollector::setAstVariableNodes(const std::map<std::string, triton::ast::AbstractNode*>& nodes) {
      this->variableNodes = nodes;
    }

  }; /* ast namespace */
}; /*triton namespace */

