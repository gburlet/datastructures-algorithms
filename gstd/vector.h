#ifndef GSTD_VECTOR
#define GSTD_VECTOR

#include <iterator>
#include <string>
#include <stdexcept>

namespace gstd {
    /*
     * Simplified implementation of the C++ STL vector class
     */
    template <class T> 
    class vector {
        public:
            typedef unsigned int uint;
            typedef T* iterator;
            typedef const T* const_iterator;

            /*
             * Default constructor. Zero-size vector.
             */
            explicit vector() 
            : data(0), lsize(0), psize(0)
            {}

            /*
             * Constructor with init size
             * logical and physical equivalent
             */
            explicit vector(uint size, const T& val = T())
            : lsize(size), psize(size) {
                data = new T[size];
                for (int i = 0; i < size; i++) {
                    data[i] = val;
                }
            }

            /*
             * Copy constructor
             * initialize from another vector's data
             */
            vector(const vector<T>& x)
            : data(0), lsize(0), psize(0)
            {
                // allocate memory and copy 
                CopyData(x);
            }

            ~vector() { delete[] data; }

            // vector logical size
            uint size() const { return lsize; }

            // vector physical size
            uint capacity() const { return psize; }
            
            // grow logical size of vector
            void resize(uint n, T val = T()) {
                reserve(n);
                for (uint i = lsize; i < n; i++) {
                    data[i] = val;
                }
                for (uint i = n; i < lsize; i++) {
                    data[i].~T();
                }
                lsize = n;
            }

            bool empty() const { return lsize == 0; }

            void reserve(uint n) {
                if (n > psize) {
                    // allocate new array
                    T* ndata = new T[n];
                    for (uint i = 0; i < lsize; i++) {
                        ndata[i] = data[i];
                    }
                    delete[] data;
                    data = ndata;
                    psize = n;
                }
            }

            // push given value to end of vector
            void push_back(const T& val) {
                if (lsize == psize) {
                    uint newSize = (psize == 0) ? 2 : 2*psize;  // min of 2 mem units in vector
                    reserve(newSize);
                }
                data[lsize] = val;
                lsize++;
            }

            // remove and and delete last element in vector
            void pop_back() {
                data[lsize-1].~T();
                lsize--;
            }

            // insert element at given iterator position
            // if you're calling this function a lot, you shouldn't be using a vector
            iterator insert(iterator position, const T& val) {
                if (position > this->end() || position < this->begin()) {
                    throw std::out_of_range("Iterator out of range."); 
                }

                if (lsize == psize) {
                    // need to allocate more space
                    uint newSize = (psize == 0) ? 2 : 2*psize;  // min of 2 mem units in vector
                    uint offset = position - this->begin();
                    reserve(newSize);
                    position = this->begin() + offset;
                }

                if (position < this->end()) {
                    // shuffle elements over
                    iterator it = this->end() - 1;
                    while (it >= position) {
                        *(it+1) = *(it);
                        it--;
                    }
                }

                *position = val;
                lsize++;

                return position;
            }

            // erase element at given iterator position
            // if you're calling this function a lot, you shouldn't be using vector
            iterator erase(iterator position) {
                if (position < this->begin() || position >= this->end()) {
                    throw std::out_of_range("Iterator out of range.");
                }

                // destroy element
                position->~T();

                iterator it = position;
                while (it < this->end()-1) {
                    *it = *(it+1);
                    it++;
                }

                lsize--;

                return position;
            }
            
            /*
             * Clears contents of vector leaving it with a size of 0.
             */
            void clear() {
                if (data) {
                    delete[] data;
                    data = 0;
                    lsize = psize = 0;
                }
            }
            
            // iterators (implicit conversion for const)
            iterator begin() { return data; }
            iterator end() { return data + lsize; }

            // assignment operator
            vector& operator=(const vector<T>& x) {
                // clean up old vector data
                clear();

                // copy over new vector data
                CopyData(x);
                
                return *this;
            }
            
            // access operator
            T& operator[](uint i) {
                if (i >= lsize) {
                    throw std::out_of_range("Index out of range.");
                }
                return data[i];
            }

            // const access operator
            const T& operator[](uint i) const {
                if (i >= lsize) {
                    throw std::out_of_range("Index out of range.");
                }
                return data[i];
            }

            // equivalence operator
            bool operator==(const vector<T>& x);

            // not equivalent operator
            bool operator!=(const vector<T>& x) {
                return !(*this == x);
            }

        private:
            T* data;        // underlying data array
            uint lsize;     // logical size
            uint psize;     // physical size

            // copy helper function
            void CopyData(const vector<T>& x) {
                lsize = x.size();
                psize = x.capacity();
                data = new T[psize];
                for (int i = 0; i < lsize; i++) {
                    data[i] = x[i];
                }
            }
    };
}

#endif
