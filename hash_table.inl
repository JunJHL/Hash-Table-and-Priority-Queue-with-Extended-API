template <typename ValueType>
bool HashTable<ValueType>::isPrime(unsigned table_size) {
    for(unsigned i = 2; i < std::sqrt(table_size); i++) {
        if(table_size % i == 0) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
bool isSamePair(Pair<ValueType> p1, Pair<ValueType> p2) {
    if(p1.key == p2.key && p1.value == p2.value && p1.stat == p2.stat) {
        return true;
    }
    return false;
}


template <typename ValueType> 
const void HashTable<ValueType>::quadraticProb(unsigned& i, unsigned& pos, unsigned key) const {
    pos = key % table_size + pow(i, 2);
    ++i;
    if(pos > table_size-1) {
        pos %= (table_size);
    }
} 

template <typename ValueType>
void HashTable<ValueType>::rehash(unsigned key, const ValueType& value) {
    num_element = 0;
    std::unique_ptr<Pair<ValueType>[]> temp;  //make a copy of old table 

    unsigned temp_size = table_size;
    table_size = table_size * 2 + 1;
    while(!isPrime(table_size)) {   // rehash table to next prime after double
        ++table_size;
    }
    // don't use function tableSize();
    temp = std::make_unique<Pair<ValueType>[]>(temp_size);
    temp = std::move(hash_table);
    hash_table = std::make_unique<Pair<ValueType>[]>(table_size);
    
    for(unsigned i = 0; i < temp_size; i++) {
        if(temp[i].stat == Status::Occupied) {
            insert(temp[i].key, temp[i].value);
        }
    }
    insert(key, value);

}

template <typename ValueType>
bool HashTable<ValueType>::insert(unsigned key, const ValueType& value) {
    unsigned i = 1;
    Pair<ValueType> pair = {key, value, Status::Occupied};

    if(get(key) != nullptr) {
        return false;
    }

    double lamb = (double)(num_element+1) / table_size;
    if(lamb > 0.5) {
       rehash(key, value);
    }else {
        unsigned pos = key % table_size;
        while(hash_table[pos].stat == Status::Occupied) {
            quadraticProb(i, pos, key);
        }
        hash_table[pos] = pair;
        ++num_element;
    }
    return true;
}

template <typename ValueType>
ValueType* HashTable<ValueType>::get(unsigned key) {
    unsigned pos = key % tableSize();
    unsigned i = 1;
    while(hash_table[pos].stat != Status::Empty) {
        if(hash_table[pos].key == key && hash_table[pos].stat != Status::Deleted) {
            return &(hash_table[pos].value);
        }
        quadraticProb(i, pos, key);
    }
    return nullptr;
}

template<typename ValueType>
const ValueType* HashTable<ValueType>::get(unsigned key) const {
    unsigned pos = key % tableSize();
    unsigned i = 1;
    while(hash_table[pos].stat != Status::Empty) {
        if(hash_table[pos].key == key && hash_table[pos].stat != Status::Deleted) {
            return &(hash_table[pos].value);
        }
        quadraticProb(i, pos, key);
    }
    return nullptr;
}

template <typename ValueType>
bool HashTable<ValueType>::update(unsigned key, const ValueType& newValue) {
    if(get(key) == nullptr) {
        return false;
    }
    *(get(key)) = newValue;
    return true;
   
}

template <typename ValueType>
bool HashTable<ValueType>::remove(unsigned key) {
    unsigned pos = key % tableSize();
    unsigned i = 1;

    if(get(key) == nullptr) {
        return false;
    }
    while(hash_table[pos].key != key) {
        quadraticProb(i, pos, key);
    }
    hash_table[pos].stat = Status::Deleted;
    --num_element;
    return true;
}

template <typename ValueType>
unsigned HashTable<ValueType>::removeAllByValue(const ValueType& value) {
    unsigned num_deleted = 0;
    for(unsigned i = 0; i < tableSize(); i++) {
        if(hash_table[i].value == value) {
            if(hash_table[i].stat == Status::Occupied) {
                hash_table[i].stat = Status::Deleted;
                ++num_deleted;
                --num_element;
            }
        }
    }
    return num_deleted;
}

template <typename ValueType>
bool HashTable<ValueType>::operator==(const HashTable& rhs) const {
    unsigned size = 0;
    if(num_element != rhs.numElements()) {
        return false;
    }

    if(table_size < rhs.tableSize()) {
        size = table_size;
    }else {
        size = rhs.tableSize();
    }

    for(unsigned i = 0; i < size; i++) {
        if(hash_table[i].stat == Status::Occupied) {
            if(rhs.get(hash_table[i].key) == nullptr) {
                return false;
            }

            unsigned pos = hash_table[i].key % rhs.tableSize();
            unsigned iter = 1; 
            while(rhs.hash_table[pos].key != hash_table[i].key) {
                quadraticProb(iter, pos, hash_table[i].key);
            }
            if(!isSamePair(hash_table[i], rhs.hash_table[pos])) {
                return false;
            }
        }
    }
    return true;
}

template <typename ValueType>
bool HashTable<ValueType>::operator!=(const HashTable& rhs) const {
    if(*this == rhs) {
        return false;
    }
    return true;
}

template <typename ValueType>
HashTable<ValueType> HashTable<ValueType>::operator+(const HashTable& rhs) const {
    HashTable<ValueType> sum_hash(table_size);

    for(unsigned i = 0; i < table_size; i++) {
        if(hash_table[i].stat == Status::Occupied) {
            sum_hash.insert(hash_table[i].key, hash_table[i].value);
        }
    }
    for(unsigned i = 0; i < rhs.table_size; i++) {
        if(rhs.hash_table[i].stat == Status::Occupied) {
            sum_hash.insert(rhs.hash_table[i].key, rhs.hash_table[i].value); 
        }
    }
    return sum_hash;
}