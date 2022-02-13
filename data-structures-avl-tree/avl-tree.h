#pragma once

#include <memory>

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
			key_type get_key() const { return m_key_; }
			value_type get_value() const { return m_value_; }

		private:
			std::int32_t get_balance_factor() const;

			void update_height();
			std::int32_t get_height() const;
			void set_height(const std::int32_t height) { m_height_ = height; }

			avl_tree_node* get_left() const { return p_left_.get(); }
			avl_tree_node* get_right() const { return p_right_.get(); }

			std::unique_ptr<avl_tree_node> move_left() { return std::move(p_left_); }
			std::unique_ptr<avl_tree_node> move_right() { return std::move(p_right_); }

			const avl_tree_node* get_const_left() const { return p_left_.get(); }
			const avl_tree_node* get_const_right() const { return p_right_.get(); }

			void set_left(const key_type& key, const value_type& value);
			void set_right(const key_type& key, const value_type& value);

			void set_left(std::unique_ptr<avl_tree_node>&& node) { p_left_ = std::move(node); }
			void set_right(std::unique_ptr<avl_tree_node>&& node) { p_right_ = std::move(node); }

		private:
			key_type m_key_{ key_type() };
			value_type m_value_{ value_type() };

		private:
			std::int32_t m_height_{ 1 };
			std::unique_ptr<avl_tree_node> p_left_{ nullptr };
			std::unique_ptr<avl_tree_node> p_right_{ nullptr };
		};

	public:
		avl_tree() = default;

	public:
		bool insert(const key_type& key, const value_type& value);
		const avl_tree_node* search(const key_type& key) const;

	private:
		void balance();

		static std::unique_ptr<avl_tree_node> balance(std::unique_ptr<avl_tree_node>&& node);

		static std::unique_ptr<avl_tree_node> rotate_left(std::unique_ptr<avl_tree_node>&& node);
		static std::unique_ptr<avl_tree_node> rotate_right(std::unique_ptr<avl_tree_node>&& node);

		static std::unique_ptr<avl_tree_node> rotate_left_right(std::unique_ptr<avl_tree_node>&& node);
		static std::unique_ptr<avl_tree_node> rotate_right_left(std::unique_ptr<avl_tree_node>&& node);


	private:
		std::unique_ptr<avl_tree_node> p_root_{ nullptr };
	};

	template <class KeyType, class ValueType>
	avl_tree<KeyType, ValueType>::avl_tree_node::avl_tree_node(const key_type& key, const value_type& value) :
		m_key_(key), m_value_(value_type(value))
	{
	}

	template <class KeyType, class ValueType>
	std::int32_t avl_tree<KeyType, ValueType>::avl_tree_node::get_balance_factor() const
	{
		const std::int32_t left_height = p_left_ != nullptr ? p_left_->get_height() : 0;
		const std::int32_t right_height = p_right_ != nullptr ? p_right_->get_height() : 0;
		return right_height - left_height;
	}

	template <class KeyType, class ValueType>
	void avl_tree<KeyType, ValueType>::avl_tree_node::update_height()
	{
		std::int32_t left_height = 0, right_height = 0;
		if (p_left_ != nullptr)
		{
			p_left_->update_height();
			left_height = p_left_->get_height();
		}

		if (p_right_ != nullptr)
		{
			p_right_->update_height();
			right_height = p_right_->get_height();
		}

		m_height_ = std::max(left_height, right_height) + 1;
	}

	template <class KeyType, class ValueType>
	std::int32_t avl_tree<KeyType, ValueType>::avl_tree_node::get_height() const
	{
		const std::int32_t left_height = p_left_ != nullptr ? p_left_->get_height() : 0;
		const std::int32_t right_height = p_right_ != nullptr ? p_right_->get_height() : 0;
		return std::max(left_height, right_height) + 1;
	}

	template <class KeyType, class ValueType>
	void avl_tree<KeyType, ValueType>::avl_tree_node::set_left(const key_type& key, const value_type& value)
	{
		p_left_ = std::unique_ptr<avl_tree_node>(new avl_tree_node(key, value));
	}

	template <class KeyType, class ValueType>
	void avl_tree<KeyType, ValueType>::avl_tree_node::set_right(const key_type& key, const value_type& value)
	{
		p_right_ = std::unique_ptr<avl_tree_node>(new avl_tree_node(key, value));
	}

	template <class KeyType, class ValueType>
	bool avl_tree<KeyType, ValueType>::insert(const key_type& key, const value_type& value)
	{
		if (p_root_ == nullptr)
		{
			p_root_ = std::unique_ptr<avl_tree_node>(new avl_tree_node(key, value));
			return true;
		}

		avl_tree_node* node = p_root_.get();
		while (true)
		{
			const key_type node_key = node->get_key();
			if (key == node_key)
				return false;

			if (key < node_key)
			{
				if (node->get_const_left() == nullptr)
				{
					node->set_left(key, value);
					break;
				}

				node = node->get_left();
				continue;
			}

			if (node->get_const_right() == nullptr)
			{
				node->set_right(key, value);
				break;
			}

			node = node->get_right();
		}

		p_root_->update_height();
		balance();

		return true;
	}

	template <class KeyType, class ValueType>
	const typename avl_tree<KeyType, ValueType>::avl_tree_node* avl_tree<KeyType, ValueType>::search(
		const key_type& key) const
	{
		const avl_tree_node* node = p_root_.get();
		while (node != nullptr)
		{
			if (node->get_key() == key)
				return node;

			node = key < node->get_key() ? node->get_const_left() : node->get_const_right();
		}

		return nullptr;
	}

	template <class KeyType, class ValueType>
	void avl_tree<KeyType, ValueType>::balance()
	{
		if (!(p_root_->get_balance_factor() < -1 || p_root_->get_balance_factor() > 1))
			return;

		p_root_ = balance(std::move(p_root_));
	}

	template <class KeyType, class ValueType>
	std::unique_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::balance(
		std::unique_ptr<avl_tree_node>&& node)
	{
		const std::int32_t balance_factor = node->get_balance_factor();
		const std::int32_t left_balance_factor = node->get_left() != nullptr ? node->get_left()->get_balance_factor() : 0;
		const std::int32_t right_balance_factor = node->get_right() != nullptr ? node->get_right()->get_balance_factor() : 0;
		if (balance_factor < -1 && left_balance_factor == -1)
			return rotate_right(std::move(node));
		else if (balance_factor > 1 && right_balance_factor == 1)
			return rotate_left(std::move(node));
		else if (balance_factor < -1 && left_balance_factor == 1)
			return rotate_left_right(std::move(node));
		else if (balance_factor > 1 && right_balance_factor == -1)
			return rotate_right_left(std::move(node));

		return std::move(node);
	}

	template <class KeyType, class ValueType>
	std::unique_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::rotate_left(
		std::unique_ptr<avl_tree_node>&& node)
	{
		std::unique_ptr<avl_tree_node> right = node->move_right();
		node->set_right(std::move(right->move_left()));
		node->set_height(node->get_height());

		right->set_left(std::move(node));
		right->set_height(right->get_height());

		return std::move(right);
	}

	template <class KeyType, class ValueType>
	std::unique_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::rotate_right(
		std::unique_ptr<avl_tree_node>&& node)
	{
		std::unique_ptr<avl_tree_node> left = node->move_left();
		node->set_left(left->move_right());
		node->set_height(node->get_height());

		left->set_right(std::move(node));
		left->set_height(left->get_height());

		return std::move(left);
	}

	template <class KeyType, class ValueType>
	std::unique_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::rotate_left_right(
		std::unique_ptr<avl_tree_node>&& node)
	{
		node->set_left(rotate_left(node->move_left()));
		node = rotate_right(std::move(node));
		node->set_height(node->get_height());

		return std::move(node);
	}

	template <class KeyType, class ValueType>
	std::unique_ptr<typename avl_tree<KeyType, ValueType>::avl_tree_node> avl_tree<KeyType, ValueType>::rotate_right_left(
		std::unique_ptr<avl_tree_node>&& node)
	{
		node->set_right(rotate_right(node->move_right()));
		node = rotate_left(std::move(node));
		node->set_height(node->get_height());

		return std::move(node);
	}
} // DS
