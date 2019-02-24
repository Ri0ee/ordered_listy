#pragma once

template<class TE>
class l_element {
public:
	l_element() {};

	l_element(TE data_, l_element<TE>* next_element_ptr_ = nullptr) : m_data(data_), m_next_element_ptr(next_element_ptr_) {}

	l_element(l_element<TE>* next_element_ptr_) : m_next_element_ptr(next_element_ptr_) {}

	~l_element() {};

	l_element<TE>*& next() {
		return m_next_element_ptr;
	}

	TE& get_data() {
		return m_data;
	}

private:
	TE m_data{};
	l_element<TE>* m_next_element_ptr = nullptr;
};

template<class TI>
class l_iterator {
public:
	l_iterator() {}

	l_iterator(const l_iterator<TI>& iterator_) {
		m_element_ptr = iterator_.element();
	}

	l_iterator(l_element<TI>* element_ptr_) : m_element_ptr(element_ptr_) {}

	~l_iterator() {}

	l_iterator<TI>& operator++() { // Prefix increment
		m_element_ptr = m_element_ptr->next();
		return *this;
	}

	l_iterator<TI> operator++(int) { // Postfix increment
		l_iterator<TI> result(*this);
		++(*this);
		return result;
	}

	TI& operator*() {
		return m_element_ptr->get_data();
	}

	bool operator==(const l_iterator<TI>& iterator_) {
		return iterator_.element() == m_element_ptr;
	}

	bool operator!=(const l_iterator<TI>& iterator_) {
		return iterator_.element() != m_element_ptr;
	}

	l_element<TI>* element() const {
		return m_element_ptr;
	}

	void swap_data(l_iterator<TI>& iterator_) {
		TI temp_data = iterator_.element()->get_data();
		iterator_.element()->get_data() = m_element_ptr->get_data();
		m_element_ptr->get_data() = temp_data;
	}

	void swap(l_iterator<TI>& iterator_) {
		l_element<TI>* temp_element_ptr = iterator_.element();
		iterator_.element() = m_element_ptr;
		m_element_ptr = temp_element_ptr;
	}

private:
	l_element<TI>* m_element_ptr = nullptr;
};

template<class TL>
class ordered_list {
public:
	ordered_list() {}

	ordered_list(ordered_list& list_) {
		for (auto it = list_.begin(); it != list_.end(); it++) 
			push(*it);
	}

	ordered_list(l_iterator<TL> begin_iterator_, l_iterator<TL> end_iterator_ = l_iterator<TL>(nullptr)) {
		for (auto it = begin_iterator_; it != end_iterator_; it++)
			push(*it);
	}

	~ordered_list() {
		clear();
	}

	// Adds data in sorted way. Needs TL type to have ">" operator implemented
	void add(TL data_) {
		if (m_head == nullptr) {
			push_front(data_);
			m_tail = m_head;
			return;
		}

		unsigned int current_pos = 0;
		for (auto it = begin(); it != end(); it++) {
			if (*it > data_) {
				insert(current_pos, data_);
				return;
			}
			current_pos++;
		}

		push(data_);
	}

	// Remove last element. Returns true if removed anything
	bool pop() {
		if (m_head == nullptr) return false;

		if (m_pre_tail != nullptr) { // If we got pre-tail element due to some actions, it is faster to pop the list in such way
			delete m_tail;
			m_pre_tail->next() = nullptr;
			m_tail = m_pre_tail;
		}
		else {
			l_element<TL>* tail = m_head;
			l_element<TL>* ptail = pre_tail();
			while (tail->next() != nullptr) { // Search for the tail and pre-tail elements
				pre_tail = tail;
				tail = tail->next();
			}

			delete tail;
			if (pre_tail != nullptr) { // If there are at least 2 elements in array
				pre_tail->next() = nullptr;
				m_tail = pre_tail;
			}
			else { // If there is only one element in array and it got deleted
				m_head = nullptr;
				m_tail = nullptr;
			}
		}

		m_pre_tail = nullptr;
		m_size--;
		return true;
	}

	// Remove first element. Returns true if removed anything
	bool pop_front() {
		if (m_head == nullptr) return false;

		l_element<TL>* temp_element = m_head->next();
		delete m_head;
		m_head = temp_element;

		m_size--;
		return true;
	}

	// Removes one element from the list. Returns true if removed anything
	bool remove(int pos_) {
		if (m_head == nullptr || pos_ < 0 || pos_ >= m_size) return false;
		if (pos_ == 0) return pop_front();
		if (pos_ == m_size - 1) return pop();

		int current_pos = 0;
		l_element<TL>* current_element = m_head;
		l_element<TL>* previous_element = nullptr;
		while (current_element->next() != nullptr && current_pos < pos_) { // Iterate to the needed element
			previous_element = current_element;
			current_element = current_element->next();
			current_pos++;
		}

		previous_element->next() = current_element->next();
		delete current_element;

		m_size--;
		return true;
	}

	// Removes all elements from the list
	void clear() {
		while (pop_front());
	}

	// Searches for the first appearance of data in list
	l_iterator<TL> find_first(TL data_) {
		for (auto it = begin(); it != end(); it++) {
			if (*it == data_) return l_iterator<TL>(it);
		}

		return end();
	}

	// Searches for the last appearance of data in list
	l_iterator<TL> find_last(TL data_) {
		l_iterator<TL> temp_iterator = end();
		for (auto it = begin(); it != end(); it++) {
			if (*it == data_) temp_iterator = it;
		}

		return temp_iterator;
	}

	// Searches for the element with minimal value
	l_iterator<TL> find_min(l_iterator<TL> begin_ = begin(), l_iterator<TL> end_ = end()) {
		l_iterator<TL> current_min(begin_);
		for (auto it = begin_; it != end_; it++) {
			if (*current_min > *it) {
				current_min = it;
			}
		}
		return current_min;
	}

	l_iterator<TL> find_max(l_iterator<TL> begin_ = begin(), l_iterator<TL> end_ = end()) {
		l_iterator<TL> current_max(begin_);
		for (auto it = begin_; it != end_; it++) {
			if (*current_max < *it) {
				current_max = it;
			}
		}
		return current_max;
	}

	// Returns amount of elements in list
	unsigned int size() {
		return m_size;
	}

	// Returns iterator of the first element of the list
	l_iterator<TL> begin() {
		return l_iterator<TL>(m_head);
	}

	// Returns iterator signalling about the end for the list
	l_iterator<TL> end() {
		return l_iterator<TL>(nullptr);
	}

	// Returns iterator of the last element of the list
	l_iterator<TL> tail() {
		return l_iterator<TL>(m_tail);
	}

	// Returns iterator of the element which stays before the tail
	l_iterator<TL> pre_tail() {
		return l_iterator<TL>(m_pre_tail);
	}

	// Returns distance between two iterators (amount of elements between them)
	int distance(l_iterator<TL> begin_iterator_, l_iterator<TL> dest_iterator_) {
		int counter = 0;
		for (auto it = begin_iterator_; it != end(); it++) {
			if (it == dest_iterator_) return counter;
			counter++;
		}

		return -1;
	}

private:
	l_element<TL>* m_head = nullptr;
	l_element<TL>* m_tail = nullptr;
	l_element<TL>* m_pre_tail = nullptr;
	unsigned int m_size = 0;

	// Push data to the end of the list
	void push(TL data_) {
		if (m_head == nullptr) {
			m_head = new l_element<TL>(data_);
			m_tail = m_head;
			m_size++;
			return;
		}

		m_tail->next() = new l_element<TL>(data_);
		m_pre_tail = m_tail;
		m_tail = m_tail->next();
		m_size++;
	}

	// Push data before the first element (head)
	void push_front(TL data_) {
		l_element<TL>* new_head = new l_element<TL>(data_, m_head);
		m_head = new_head;

		m_size++;
	}

	// Inserts data into the list at pos_
	void insert(unsigned int pos_, TL data_) {
		if (m_head == nullptr || pos_ == 0) {
			push_front(data_);
			return;
		}

		if (pos_ >= m_size) {
			push(data_);
			return;
		}

		unsigned int current_pos = 0;
		l_element<TL>* current_element = m_head;
		l_element<TL>* previous_element = nullptr;
		while (current_element->next() != nullptr && current_pos < pos_) {
			previous_element = current_element;
			current_element = current_element->next();
			current_pos++;
		}

		previous_element->next() = new l_element<TL>(data_, current_element);

		m_size++;
	}
};