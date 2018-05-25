#ifndef _CLIFF_QUALIFIED_IDENTIFIER_H
#define _CLIFF_QUALIFIED_IDENTIFIER_H

#include <map>

namespace cliff {

	template<typename Info>
	class QualifiedIdentifierNode {

		template<typename T>
		friend class QualifiedIdentifier;

	public:
		template<typename... Args>
		QualifiedIdentifierNode(const std::string& name, QualifiedIdentifierNode<Info>* parent, Args&&... args) : _name(name), _info(std::forward<Args>(args)...), _parent(parent), _children() {

		}

		template<typename... Args>
		QualifiedIdentifierNode(const char* name, QualifiedIdentifierNode<Info>* parent, Args&&... args) : _name(name), _info(std::forward<Args>(args)...), _parent(parent), _children() {

		}

	private:
		std::string _name;
		Info _info;
		QualifiedIdentifierNode<Info>* _parent;
		std::map<std::string, QualifiedIdentifierNode<Info>> _children;

	};

	template<typename Info>
	class QualifiedIdentifier {

	public:
		//QualifiedIdentifier(const char* identifier);
		//QualifiedIdentifier(QualifiedIdentifier* prefix, const char* identifier);

		//static void parse(const char* str);
		QualifiedIdentifier(QualifiedIdentifierNode<Info>& node) : _node(&node) {

		}

		QualifiedIdentifier(const QualifiedIdentifier<Info>& that) : _node(that._node) {

		}

		QualifiedIdentifier& operator=(const QualifiedIdentifier<Info>& that) {
			_node = that._node;
			return *this;
		}

		template<typename... Args>
		QualifiedIdentifier<Info> create_child(const char* identifier, Args&&... args) {
			return QualifiedIdentifier<Info>(_node->_children.emplace(std::piecewise_construct, std::forward_as_tuple(identifier), std::forward_as_tuple(identifier, _node, std::forward<Args>(args)...)).first->second);

		}

		template<typename... Args>
		QualifiedIdentifier<Info> create_child(const std::string& identifier, Args&&... args) {
			return QualifiedIdentifier<Info>(_node->_children.emplace(std::piecewise_construct, std::forward_as_tuple(identifier), std::forward_as_tuple(identifier, _node, std::forward<Args>(args)...)).first->second);
		}

		size_t children_number() const {
			return _node->_children.size();
		}

		std::map<std::string, QualifiedIdentifierNode<Info>>& children() {
			return _node->_children;
		}

		const std::map<std::string, QualifiedIdentifierNode<Info>>& children() const {
			return _node->_children;
		}

		Info& info() {
			return _node->_info;
		}

		const Info& info() const {
			return _node->_info;
		}

		bool has_child(const char* identifier) {
			return _node->_children.find(identifier) != std::end(_node->_children);
		}

		bool has_child(const std::string& identifier) {
			return _node->_children.find(identifier) != std::end(_node->_children);
		}

		QualifiedIdentifier<Info> get_child(const char* identifier) {
			return QualifiedIdentifier<Info>(_node->_children.find(identifier)->second);
		}

		QualifiedIdentifier<Info> get_child(const std::string& identifier) {
			return QualifiedIdentifier<Info>(_node->_children.find(identifier)->second);
		}

		std::string to_string() const {
			std::string str(_node->_name);

			QualifiedIdentifierNode<Info>* current_node = _node->_parent;

			while(current_node != nullptr && current_node->_name != "") {
				str = current_node->_name+"."+str;
				current_node = current_node->_parent;
			}

			return str;
		}

	private:
		QualifiedIdentifierNode<Info>* _node;

	};
}
#endif
