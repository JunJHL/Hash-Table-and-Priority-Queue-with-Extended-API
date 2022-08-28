#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <iostream>
#include <memory>
#include "hash_table.hpp"

/**
 * Implementation of a priority queue that supports the
 * extended API discussed during lecture. This priority
 * queue maps unsigned values to instances of ValueType.
 * You are required to use a hash table to efficiently
 * support the operations of the extended API.
 *
 * As with the hash table, any use of the term "element" here
 * refers to a key-value pair.
 *
 * As with the hash table (see the block comment right before the
 * definition of class HahsTable), any mention of "constant time" in
 * double quotes means "amortized constant time on average",
 * i.e. you should use hashing / the required hash table
 * to speed things up and not iterate through the entire underlying
 * binary heap array unnecessarily.
 *
 * The priority queue's underlying implementation is
 * required to be a binary min heap and the hash table
 * that you implement in the first part of this assignment.
 */

template <typename ValueType>
struct KeyValuePair{
    unsigned key;
    ValueType value;
};

template <typename ValueType>
class PriorityQueue
{
public:
    /**
     * Creates a priority queue that can have at most @maxSize elements.
     *
     * Throws std::runtime_error if @maxSize is 0.
     */
    explicit PriorityQueue(unsigned maxSize) : ht(HashTable<unsigned>(nextPrime(maxSize))) {
        if(maxSize == 0) {
            throw std::runtime_error("Max Size can't be zero");
        }
        binary_heap = std::make_unique<KeyValuePair<ValueType>[]>(maxSize+1);
        max_size = maxSize;
        num_element = 0;
    }
    
    ~PriorityQueue() {}

    /**
     * Makes the underlying implementation details (including the max size) look
     * exactly the same as that of @rhs.
     */
    PriorityQueue(const PriorityQueue& rhs) : ht(HashTable<unsigned>(nextPrime(rhs.max_size))) {
        max_size = rhs.max_size;
        num_element = rhs.num_element;
        binary_heap = std::make_unique<KeyValuePair<ValueType>[]>(max_size+1);
        for(unsigned i = 1; i <= rhs.num_element; i++) {
            binary_heap[i].key = rhs.binary_heap[i].key;
            binary_heap[i].value = rhs.binary_heap[i].value;
            ht.insert(rhs.binary_heap[i].key, i);
        }
    }
    PriorityQueue& operator=(const PriorityQueue& rhs) {
        max_size = rhs.max_size;
        num_element = rhs.num_element;
        ht = HashTable<unsigned>(nextPrime(rhs.max_size));
        binary_heap = std::make_unique<KeyValuePair<ValueType>[]>(max_size+1);
        for(unsigned i = 1; i <= rhs.num_element; i++) {
            binary_heap[i].key = rhs.binary_heap[i].key;
            binary_heap[i].value = rhs.binary_heap[i].value;
            ht.insert(rhs.binary_heap[i].key, i);
        }
        return *this;
    }

    /**
     * Takes the underlying implementation details of @rhs
     * and gives them to "this" object.
     * After this, @rhs should be in a "moved from" state.
     */
    PriorityQueue(PriorityQueue&& rhs) noexcept : ht(std::move(rhs.ht)){
        max_size = rhs.max_size;
        num_element = rhs.num_element;
        binary_heap = std::move(rhs.binary_heap);
        rhs.binary_heap = nullptr;
        rhs.num_element = 0;
    }

    PriorityQueue& operator=(PriorityQueue&& rhs) noexcept {
        max_size = rhs.max_size;
        num_element = rhs.num_element;
        binary_heap = std::move(rhs.binary_heap);
        ht = std::move(rhs.ht);
        rhs.binary_heap = nullptr;
        rhs.num_element = 0;
        return *this;
    }

    /**
     * Both of these must run in constant time.
     */
    unsigned numElements() const {
        return num_element;
    }
    unsigned maxSize() const {
        return max_size;
    }

    /**
     * Print the underlying heap level-by-level.
     * See prog_hw4.pdf for how exactly this should look.
     */
    friend std::ostream& operator<<(
        std::ostream& os,
        const PriorityQueue<ValueType>& pq)
    {
        // TODO: Implement this method.
        unsigned i_double = 1;
        for(unsigned i = 1; i <= pq.num_element; i++) {
            if(i == i_double * 2) {
                os << "\n";
                i_double *= 2;
            }
            os << "(" << pq.binary_heap[i].key << "," << pq.binary_heap[i].value << ") ";
        }
        os << "\n";
        return os;
    }

    /**
     * Inserts a key-value pair mapping @key to @value into
     * the priority queue.
     *
     * Returns true if success.
     * In this case, must run in logarithmic time.
     *
     * Returns false if @key is already in the priority queue
     * or if max size would be exceeded.
     * (In either of these cases, the insertion is not performed.)
     * In this case, must run in "constant time".
     */
    bool insert(unsigned key, const ValueType& value);

    /**
     * Returns key of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function must run in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const unsigned* getMinKey() const;

    /**
     * Returns value of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function must run in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const ValueType* getMinValue() const;

    /**
     * Removes the root of the priority queue.
     *
     * This function must run in logarithmic time.
     *
     * Returns true if success.
     * Returns false if priority queue is empty, i.e. nothing to delete.
     */
    bool deleteMin();

    /**
     * Returns address of the value that @key is mapped to in the priority queue.
     *
     * These functions must run in "constant time".
     *
     * Returns null pointer if @key is not in the table.
     */
    ValueType* get(unsigned key);
    const ValueType* get(unsigned key) const;

    /**
     * Subtracts/adds @change from/to the key of
     * the element that has key @key.
     *
     * These functions must run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if any of the following:
     * - @change is 0.
     * - @key not found.
     * - If the change would lead to a duplicate key.
     *
     * The function does not do anything about  overflow/underflow.
     * For example, an operation like decreaseKey(2, 10)
     * has an undefined effect.
     */
    bool decreaseKey(unsigned key, unsigned change);
    bool increaseKey(unsigned key, unsigned change);

    /**
     * Removes element that has key @key.
     *
     * These functions must run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if @key not found.
     */
    bool remove(unsigned key);

private:
    // TODO: Your members here.
    std::unique_ptr<KeyValuePair<ValueType>[]> binary_heap;
    HashTable<unsigned> ht;
    unsigned max_size;
    unsigned num_element;

    void swap(unsigned pos_1, unsigned pos_2);
    unsigned percolate(unsigned pos);
    bool isPrime(unsigned table_size);
    unsigned nextPrime(unsigned maxSize);
};

#include "priority_queue.inl"
#endif  // PRIORITY_QUEUE_HPP
