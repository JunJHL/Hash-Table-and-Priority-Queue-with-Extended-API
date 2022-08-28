template <typename ValueType>
bool PriorityQueue<ValueType>::isPrime(unsigned table_size) {
    for(unsigned i = 2; i < std::sqrt(table_size); i++) {
        if(table_size % i == 0) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
unsigned PriorityQueue<ValueType>::nextPrime(unsigned maxSize) {
    unsigned table_size = maxSize;
    while(!isPrime(table_size)) {
        ++table_size;
    }
    return table_size;
}

template <typename ValueType>
void PriorityQueue<ValueType>::swap(unsigned pos_1, unsigned pos_2) {
    KeyValuePair<ValueType> temp = binary_heap[pos_1];
    binary_heap[pos_1] = binary_heap[pos_2];
    binary_heap[pos_2] = temp;
}

template <typename ValueType>
unsigned PriorityQueue<ValueType>::percolate(unsigned pos) {
    unsigned key_pos = pos;

    while((key_pos/2 >= 1 && binary_heap[key_pos].key < binary_heap[key_pos/2].key) || 
          (key_pos*2 <= num_element && binary_heap[key_pos].key > binary_heap[key_pos*2].key) ||
          (key_pos*2+1 <= num_element && binary_heap[key_pos].key > binary_heap[key_pos*2+1].key)) {
        if(binary_heap[key_pos].key < binary_heap[key_pos/2].key) {   // percolate up
            swap(key_pos, key_pos/2);
            ht.update(binary_heap[key_pos].key, key_pos);
            ht.update(binary_heap[key_pos/2].key, key_pos/2);
            key_pos /= 2;
        //  percolate down
        }else if(key_pos*2 <= num_element && 
                key_pos*2+1 <= num_element && 
                binary_heap[key_pos].key > binary_heap[key_pos*2].key && 
                binary_heap[key_pos].key > binary_heap[key_pos*2+1].key) {  

            if(binary_heap[key_pos*2].key < binary_heap[key_pos*2+1].key) {
                swap(key_pos, key_pos*2);
                ht.update(binary_heap[key_pos].key, key_pos);
                ht.update(binary_heap[key_pos*2].key, key_pos*2);
                key_pos *= 2;
            }else if(binary_heap[key_pos*2].key > binary_heap[key_pos*2+1].key){
                swap(key_pos, key_pos*2+1);
                ht.update(binary_heap[key_pos].key, key_pos);
                ht.update(binary_heap[key_pos*2+1].key, key_pos*2+1);
                key_pos = key_pos * 2 + 1;
            }
        }else if(binary_heap[key_pos].key > binary_heap[key_pos*2].key) {
            swap(key_pos, key_pos*2);
            ht.update(binary_heap[key_pos].key, key_pos);
            ht.update(binary_heap[key_pos*2].key, key_pos*2);
            key_pos *= 2;
        }else if(binary_heap[key_pos].key > binary_heap[key_pos*2+1].key) {
            swap(key_pos, key_pos*2+1);
            ht.update(binary_heap[key_pos].key, key_pos);
            ht.update(binary_heap[key_pos*2+1].key, key_pos*2 + 1);
            key_pos = key_pos * 2 + 1;
        }
    }
    return key_pos;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::insert(unsigned key, const ValueType& value) {
    KeyValuePair<ValueType> pair = {key, value};
    unsigned key_pos = 1;
    
    if(get(key) != nullptr || num_element + 1 > max_size) {
        return false;
    }
    binary_heap[num_element+1] = pair;
    ++num_element;
    key_pos = percolate(num_element);
    ht.insert(key, key_pos);

    return true;
}

template <typename ValueType>
const unsigned* PriorityQueue<ValueType>::getMinKey() const {
    return &(binary_heap[1].key);
}

template <typename ValueType>
const ValueType* PriorityQueue<ValueType>::getMinValue() const {
    return &(binary_heap[1].value);
}

template <typename ValueType>
bool PriorityQueue<ValueType>::deleteMin() {
    unsigned key_pos = 1;

    if(num_element == 0) {
        return false;
    }
    ht.remove(binary_heap[1].key);
    binary_heap[1] = binary_heap[num_element];
    --num_element;
    key_pos = percolate(1);

    return true;
}

template <typename ValueType>
ValueType* PriorityQueue<ValueType>::get(unsigned key) {
    if(ht.get(key) == nullptr) {
        return nullptr;
    }
    return &(binary_heap[*(ht.get(key))].value);
}

template <typename ValueType>
const ValueType* PriorityQueue<ValueType>::get(unsigned key) const {
    if(ht.get(key) == nullptr) {
        return nullptr;
    }
    return &(binary_heap[*(ht.get(key))].value);
}

template <typename ValueType>
bool PriorityQueue<ValueType>::decreaseKey(unsigned key, unsigned change) {
    if(change == 0 || ht.get(key) == nullptr) {
        return false;
    }

    unsigned pos = *(ht.get(key));
    if(ht.get(binary_heap[pos].key - change) != nullptr) {
        return false;
    }
    ht.remove(key);
    binary_heap[pos].key -= change;
    pos = percolate(pos);
    ht.insert(binary_heap[pos].key, pos);

    return true;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::increaseKey(unsigned key, unsigned change) {
    if(change == 0 || ht.get(key) == nullptr) {
        return false;
    }

    unsigned pos = *(ht.get(key));
    if(ht.get(binary_heap[pos].key + change) != nullptr) {
        return false;
    }
    ht.remove(key);
    binary_heap[pos].key += change;
    pos = percolate(pos);
    ht.insert(binary_heap[pos].key, pos);

    return true;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::remove(unsigned key) {
    if(ht.get(key) == nullptr) {
        return false;
    }

    unsigned pos = *(ht.get(key));
    ht.remove(key);
    binary_heap[pos] = binary_heap[num_element];
    --num_element;
    pos = percolate(pos);
    ht.update(key, pos);

    return true;
}