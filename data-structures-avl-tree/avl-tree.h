#pragma once

#include <memory>
#include <string>

namespace ds
{
	template<class KeyType, class ValueType>
	class avl_tree
	{
		using key_type = KeyType;
		using value_type = ValueType;

	public:
		class avl_tree_node
		{
			friend class avl_tree;

		public:
			avl_tree_node(const key_type& key, const value_type& value);

		public:
			const key_type& get_key() { return m_key_; }
			const value_type& get_value() { return m_value_; }

		private:
			std::int32_t get_balance_factor();

			std::int32_t get_height() const { return m_height_; }
			void set_height(const std::int32_t height) { m_height_ = height; }

			std::shared_ptr<avl_tree_node> get_left() const { return p_left_; }
			void set_left(const std::shared_ptr<avl_tree_node>& node) { p_left_ = node; }

			std::shared_ptr<avl_tree_node> get_right() const { return p_right_; }
			void set_right(const std::shared_ptr<avl_tree_node>& node) { p_right_ = node; }

		private:
			key_type m_key_{ key_type() };
			value_type m_value_{ value_type() };

		private:
			std::int32_t m_height_{ 1 };
			std::shared_ptr<avl_tree_node> p_left_{ nullptr }, p_right_{ nullptr };
		};

	public:
		avl_tree() = default;

	public:
		void insert(const key_type& key, const value_type& value);
		std::shared_ptr<avl_tree_node> search(const key_type& key);

	private:
		static std::shared_ptr<avl_tree_node> balance(std::shared_ptr<avl_tree_node> node);

		static std::shared_ptr<avl_tree_node> rotate_left(std::shared_ptr<avl_tree_node> node);
		static std::shared_ptr<avl_tree_node> rotate_right(std::shared_ptr<avl_tree_node> node);

		static std::shared_ptr<avl_tree_node> rotate_left_right(std::shared_ptr<avl_tree_node> node);
		static std::shared_ptr<avl_tree_node> rotate_right_left(std::shared_ptr<avl_tree_node> node);

	private:
		std::shared_ptr<avl_tree_node> p_root_{ nullptr };
	};

	template <class ValueType, class KeyType>
	avl_tree<ValueType, KeyType>::avl_tree_node::avl_tree_node(const key_type& key, const value_type& value) :
		m_key_(key_type(key)), m_value_(value_type(value))
	{
	}

	template <class KeyType, class ValueType>
	std::int32_t avl_tree<KeyType, ValueType>::avl_tree_node::get_balance_factor()
	{
		const std::int32_t left_height = p_left_ != nullptr ? p_left_->m_height_ : 0;
		const std::int32_t right_height = p_right_ != nullptr ? p_right_->m_height_ : 0;
		return right_height - left_height;
	}

	template <class ValueType, class KeyType>
	void avl_tree<ValueType, KeyType>::insert(const key_type& key, const value_type& value)
	{
		if (p_root_ == nullptr)
		{
			p_root_ = std::make_shared<avl_tree_node>(key, value);
			return;
		}

		std::shared_ptr<avl_tree_node> node = p_root_;
		while (true)
		{
			// if (key == node->get_key())
			// 	throw std::exception("the value with this key is already in the tree");

			if (key < node->get_key())
			{
				if (node->get_left() == nullptr)
				{
					node->set_left(std::make_shared<avl_tree_node>(key, value));
					break;
				}

				node = node->get_left();
				continue;
			}

			if (node->get_right() == nullptr)
			{
				node->set_right(std::make_shared<avl_tree_node>(key, value));
				break;
			}

			node = node->get_right();
		}

		//const std::int32_t left_height = node->get_left() != nullptr ? node->get_left()->get_height() : 0;
		//const std::int32_t right_height = node->get_right() != nullptr ? node->get_right()->get_height() : 0;
		//node->set_height(std::max(left_height, right_height) + 1);

		//const std::int32_t balance_factor = p_root_->get_balance_factor();
		//if (balance_factor < -1 || balance_factor > 1)
		//	p_root_ = balance(p_root_);
	}

	template <class KeyType, class ValueType>
	std::shared_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::search(
		const key_type& key)
	{
		std::shared_ptr<avl_tree_node> node = p_root_;
		while (node != nullptr)
		{
			if (node->get_key() == key)
				return std::make_shared<avl_tree_node>(node->get_key(), node->get_value());

			node = key < node->get_key() ? node->get_left() : node->get_right();
		}

		return nullptr;
	}

	template <class KeyType, class ValueType>
	std::shared_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::balance(
		std::shared_ptr<avl_tree_node> node)
	{
		const std::int32_t balance_factor = node->get_balance_factor();
		const std::int32_t left_balance_factor = node->get_left() != nullptr ? node->get_left()->get_balance_factor() : 0;
		const std::int32_t right_balance_factor = node->get_right() != nullptr ? node->get_right()->get_balance_factor() : 0;
		if (balance_factor < -1 && left_balance_factor == -1)
			return rotate_right(node);
		else if (balance_factor > 1 && right_balance_factor == 1)
			return rotate_left(node);
		else if (balance_factor < -1 && left_balance_factor == 1)
			return rotate_left_right(node);
		else if (balance_factor > 1 && right_balance_factor == -1)
			return rotate_right_left(node);

		return node;
	}

	template <class KeyType, class ValueType>
	std::shared_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::rotate_left(
		std::shared_ptr<avl_tree_node> node)
	{
		std::shared_ptr<avl_tree_node> right = node->get_right();
		node->set_right(right->get_left());
		right->set_left(node);

		std::int32_t left_height = node->get_left() != nullptr ? node->get_left()->get_height() : 0;
		std::int32_t right_height = node->get_right() != nullptr ? node->get_right()->get_height() : 0;
		node->set_height(std::max(left_height, right_height) + 1);

		left_height = right->get_left() != nullptr ? right->get_left()->get_height() : 0;
		right_height = right->get_right() != nullptr ? right->get_right()->get_height() : 0;
		right->set_height(std::max(left_height, right_height) + 1);

		return right;
	}

	template <class KeyType, class ValueType>
	std::shared_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::rotate_right(
		std::shared_ptr<avl_tree_node> node)
	{
		std::shared_ptr<avl_tree_node> left = node->get_left();
		node->set_left(left->get_right());
		left->set_right(node);

		std::int32_t left_height = node->get_left() != nullptr ? node->get_left()->get_height() : 0;
		std::int32_t right_height = node->get_right() != nullptr ? node->get_right()->get_height() : 0;
		node->set_height(std::max(left_height, right_height));

		left_height = left->get_left() != nullptr ? left->get_left()->get_height() : 0;
		right_height = left->get_right() != nullptr ? left->get_right()->get_height() : 0;
		left->set_height(std::max(left_height, right_height) + 1);

		return left;
	}

	template <class KeyType, class ValueType>
	std::shared_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::
		rotate_left_right(std::shared_ptr<avl_tree_node> node)
	{
		node->set_left(rotate_left(node->get_left()));
		node = rotate_right(node);

		const std::int32_t left_height = node->get_left() != nullptr ? node->get_left()->get_height() : 0;
		const std::int32_t right_height = node->get_right() != nullptr ? node->get_right()->get_height() : 0;
		node->set_height(std::max(left_height, right_height) + 1);

		return node;
	}

	template <class KeyType, class ValueType>
	std::shared_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::
		rotate_right_left(std::shared_ptr<avl_tree_node> node)
	{
		node->set_right(rotate_right(node->get_right()));
		node = rotate_left(node);

		const std::int32_t left_height = node->get_left() != nullptr ? node->get_left()->get_height() : 0;
		const std::int32_t right_height = node->get_right() != nullptr ? node->get_right()->get_height() : 0;
		node->set_height(std::max(left_height, right_height) + 1);

		return node;
	}
} // DS
