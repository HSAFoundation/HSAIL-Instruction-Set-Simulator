//depot/stg/hsa/drivers/hsa/api/common/hsacommon.h#10 - edit change 813963 (text)
#ifndef _HSACOMMON_H_
#define _HSACOMMON_H_


#if !defined(AMD_AMP_HSA_INCLUDES)
#include <stdint.h>
#include <vector>
#include <utility>
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)
#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
        #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
        #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
    #define DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__ ((visibility ("default")))
        #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
    #endif
#endif

#define AMD_DEVICE_TYPE_UNKNOWN                      0
#define AMD_DEVICE_TYPE_DEFAULT                      (1 << 0)
#define AMD_DEVICE_TYPE_CPU                          (1 << 1)
#define AMD_DEVICE_TYPE_GPU                          (1 << 2)
#define AMD_DEVICE_TYPE_ACCELERATOR                  (1 << 3)
#define AMD_DEVICE_TYPE_HSA                          (1 << 4)
#define AMD_DEVICE_TYPE_ALL                          0xFFFFFFFF

// all the items shared between namespace hsacore and namespace hsa go to this namespace
namespace hsacommon
{

// TODO: Replace out_of_range with an HSART exception
/**
 * @brief Class hsa::vector replaces std::vector container.
 * @tparam T Type of object
 */
template<typename T>
class vector
{
public:
    typedef T* iterator;
    typedef T* const_iterator;

    /**
     * @brief Default constructor, initializes member variables to 0.
     */
    vector() : items_(0), data_(0), capacity_(0), size_(0) { }

    /**
     * @brief Destructor, destroys vector.
     */
    ~vector()
    {
        try
        {
            clear();
            delete[] data_;
        }
        catch(...){
            delete[] data_;
        }
    }

    /**
     * @brief Copy constructor
     * @param rhs Vector to be copied to current memory
     *        location.
     */
    vector(const vector& rhs) :
        items_(0), data_(0), capacity_(0), size_(0)
    {
        if(rhs.size_ != 0){
            assign(rhs.begin(), rhs.end());
        }
    }

    /**
     * @brief Copy Operator function, assigns a copy of
     *        rhs as content of vector object.
     * @return returns *this, vector object with copied content.
     * @param rhs Vector to be copied to current memory
     *        location.
     */
    const vector& operator=(const vector& rhs)
    {
        if(this == &rhs){
            return *this;
        }
        if(rhs.size_ != 0){
            assign(rhs.begin(), rhs.end());
        }
        else {
            clear();
        }
        return *this;
    }

    /**
     * @brief Function pushes all elements between start and
     *        end locations onto the vector.
     * @tparam Iterator Type of iterator
     * @param start Iterator representing start of source.
     *        Must be at least a forward iterator.
     * @param end Iterator representing end of source.
     */
    template <class Iterator> void assign(Iterator start, Iterator end)
    {
        clear();
        while(start != end){
            push_back(*start);
            start++;
        }
    }

    /**
     * @brief Function returns the number of elements in the vector.
     * @return returns the number of elements contained in the vector.
     */
    size_t size() const
    {
        return size_;
    }

    /**
     * @brief Function returns the number of items worth of
     *        memory allocated for the vector.
     * @return returns the number of items worth of memory
     *        allocated for the vector.
     * @note Capacity will be >=the size of the vector.
     */
    size_t capacity() const
    {
        return capacity_;
    }

    /**
     * @brief Function returns true if the vector is empty.
     * @return returns true only if the vector is empty.
     */
    bool empty() const
    {
        return (size_ == 0);
    }

    /**
     * @brief Function empties the vector and destructs each element
     *        as it is removed.
     * @note Memory is not freed by clear. Only destruction of
     *        the vector will deallocate memory.
     * @note Capacity will be >= the size of the vector.
     */
    void clear()
    {
        while(size_ > 0){
            pop_back();
        }
    }

    /**
     * @brief Function removes and destructs the last element of a
     *        non-empty vector.
     * @note Capacity will not be changed.
     */
    void pop_back()
    {
        if(size_ > 0) {
            items_[size_ -1].~T();
            size_--;
        }
    }

    /*
     * @brief Function adds a new element at the end of the vector.
     *        If the vector is full, the capacity is doubled before
     *        the element is added.
     * @param item The item to be added to the end of the vector.
     */
    void push_back(const T& item)
    {
        size_t itemSize = sizeof(T);

        if(capacity_ == 0){
            capacity_ = 1;

            data_ = new char[capacity_ * itemSize];
            items_ = reinterpret_cast<T*>(data_);
        }
        if(size_ == capacity_) {
            capacity_ *= 2;
            T* prevItems( items_ ); ///< Pointer to current buffer of items to be copied to a new location
            char *prevData( data_ ); ///< Point to previous location of data_ char buffer
            data_ = new char[capacity_ * itemSize];
            items_ = reinterpret_cast<T*>(data_);
            for(size_t cpyCount=0; cpyCount < size_; cpyCount++){
                new (&items_[cpyCount]) T(prevItems[cpyCount]);
            }
            // TODO: memory leak in case of exceptions. Make RAII.
            delete[] prevData;
        }
        new(&items_[size_]) T(item);
        size_++;
    }

    /**
     * @brief Function returns the reference to the last
     *        element in the vector.
     * @return returns the reference to the last element
     *        in the vector.
     * @throw Throw out_of_range exception when the vector is
     *        empty.
     */
    T& back()
    {
        if(size_ <= 0) {
            throw std::out_of_range("vector::back() called on empty hsa::vector.");
        }
        return (items_[size_-1]);
    }

    /**
     * @brief Constant function returns a constant reference to the
     *        last element in the vector.
     * @return returns a constant reference to the last element
     *        in the vector.
     * @throw Throw out_of_range exception when the vector is
     *        empty.
     */
    const T& back() const
    {
        if(size_ <= 0) {
            throw std::out_of_range("vector::back() called on empty hsa::vector.");
        }
        return (items_[size_-1]);
    }

    /**
     * @brief Function returns the reference to the element
     *        at index n in the vector.
     * @return returns the reference to the element at index
     *        n in the vector.
     * @param n The index of the element being returned.
     */
    T& operator[] (size_t n)
    {
        //TODO Assert for out_of_range
        return items_[n];
    }

    /**
     * @brief Constant function returns a constant reference to the
     *        element at index n in the vector.
     * @return returns a constant reference to the element at index
     *        n in the vector.
     * @param n The index of the element being returned.
     */
    const T& operator[] (size_t n) const
    {
        //TODO Assert for out_of_range
        return items_[n];
    }

    /**
     * @brief Function returns the reference to the element
     *        at index n in the vector
     * @return returns the reference to the element at
     *        index n in the vector
     * @throw Throw out_of_range exception when index n is
     *        less than 0 or greater than the vector size.
     * @param n The index of the element being returned.
     */
    T& at(size_t n)
    {
        if(n >= size_){
            throw std::out_of_range("vector::at() called on element outside hsa::vector.");
        }
        return items_[n];
    }

    /**
     * @brief Constant function returns a constant reference to the
     *        element at index n in the vector
     * @return returns a constant reference to the element at
     *        index n in the vector
     * @throw Throw out_of_range exception when index n is less
     *        than 0 or greater than the vector size.
     * @param n The index of the element being returned.
     */
    const T& at(size_t n) const
    {
        if(n >= size_){
            throw std::out_of_range("vector::at() called on element outside hsa::vector.");
        }
        return items_[n];
    }

    /**
     * @brief Function returns an iterator referring to
     *        the first element in the vector.
     * @return returns an iterator referring to the first
     *        element in the vector.
     */
    iterator begin()
    {
        return items_;
    }

    /**
     * @brief Constant function returns a constant iterator referring to
     *        the first element in the vector.
     *  @return returns a constant iterator referring to the first
     *        element in the vector.
     */
    const_iterator begin() const
    {
        return cbegin();
    }

    /**
     * @brief Constant function returns a constant iterator referring to
     *        the first element in the vector.
     * @return returns a constant iterator referring to the first
     *        element in the vector.
     */
    const_iterator cbegin() const
    {
        return items_;
    }

    /**
     * @brief Function returns an iterator referring to the
     *        element following the last element in the vector.
     * @return returns an iterator referring to the element
     *        following the last element in the vector.
     */
    iterator end()
    {
        return items_ + size_;
    }

    /**
     * @brief Constant function returns a constant iterator referring
     *        to the element following the last element in the
     *        vector.
     * @return returns a constant iterator referring to the element
     *        following the last element in the vector.
     */
    const_iterator end() const
    {
        return cend();
    }

    /**
     * @brief Constant function returns a constant iterator referring
     *        to the element following the last element in the
     *        vector.
     * @return returns a constant iterator referring to the element
     *        following the last element in the vector.
     */
    const_iterator cend() const
    {
        return items_ + size_;
    }

private:
    T*    items_; ///< Pointer to buffer of items of type T, is an alias of data_
    char* data_; ///< Pointer to buffer of items of type char
    size_t    capacity_; ///< Capacity of the vector
    size_t    size_; ///< Number of items contained in the vector

};//end Vector

/**
 * @brief Specifies the different timeout periods supported
 * by Hsa Runtime for event wait
 *
 * @note: The values of this enum must match corresponding
 * values defined in HsaThk layer interface.
 */
typedef enum {

    /*
     * Specifies Zero milliseconds timeout. The caller
     * will return immediately if the underlying event
     * is not signalled.
     */
    HSA_EVENT_TIMEOUT_IMMEDIATE = 0,

    /*
     * Specifies a blocking call to wait. The caller will
     * return only upon the signalling of the underlying
     * event object.
     */
    HSA_EVENT_TIMEOUT_INFINITE = 0xFFFFFFFF

} HsaEventWaitTime;

/**
 * @brief Specifies the different return values possible from
 * a call to Event Wait.
 */
typedef enum {

    /*
     * Indicates return from wait due to timeout i.e.
     * event object did not get signalled.
     */
    HSA_EVENT_WAIT_SUCCESS = 0,

    /*
     * Indicates return from wait due to event object
     * being signalled.
     */
    HSA_EVENT_WAIT_TIMEOUT = 1,

} HsaEventWaitReturn;

/**
 * @brief Specifies different memory types according to HSAIL spec 
 *        that can be allocated through the runtime.
 */
typedef enum
{
    MEMORY_TYPE_UNDEFINED = 0,

    /* 
     * @brief Memory visible to all work-groups and agents. 
     */
    MEMORY_TYPE_GLOBAL = 1,

    /* 
     * @brief Memory visible only within a single work-group. 
     */
    MEMORY_TYPE_GROUP = 2,
} MemoryType;

/**
 * @brief Specifies different kinds of backing storage for 
 *        MEMORY_TYPE_GLOBAL.
 */
typedef enum
{
    HEAP_TYPE_UNDEFINED = 0,

    /*
     * @brief GLOBAL memory is allocated in system physical memory.
     */
    HEAP_TYPE_SYSTEM = (1U << 0), 

    /*
     * @brief GLOBAL memory is allocated in device-global memory.
     */
    HEAP_TYPE_DEVICE = (1U << 1)
} HeapType;

typedef enum
{
    /**
     * Memory option used for requesting memory with caching disabled. This
     * option is mutually exclusive with the MEMORY_OPTION_WRITE_COMBINED option. 
     */
    MEMORY_OPTION_UNCACHED = (1U << 8),

    /**
     * Memory option used for requesting memory that is write combined. 
     */
    MEMORY_OPTION_WRITE_COMBINED = (1U << 9),

    /**
     * Memory option used for requesting memory that won't be paged-out to disk.
     * This option is mutually exclusive with the MEMORY_OPTION_PAGEABLE option. 
     */
    MEMORY_OPTION_NONPAGEABLE = (1U << 10),

    /**
     * Memory option used for declaring that the requested memory will be both
     * read and written by the device. This option is mutually exclusive with
     * the MEMORY_OPTION_DEVICE_NO_ACCESS, MEMORY_OPTION_DEVICE_READ_ONLY, and
     * MEMORY_OPTION_DEVICE_WRITE_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_READ_WRITE = (1U << 11),

    /**
     * Memory option used for declaring that the requested memory will only be
     * both read by the device. This option is mutually exclusive with the
     * MEMORY_OPTION_DEVICE_NO_ACCESS, MEMORY_OPTION_DEVICE_READ_WRITE, and
     * MEMORY_OPTION_DEVICE_WRITE_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_READ_ONLY = (1U << 12),

    /**
     * Memory option used for declaring that the requested memory will only be
     * both written by the device. This option is mutually exclusive with the
     * MEMORY_OPTION_DEVICE_NO_ACCESS, MEMORY_OPTION_DEVICE_READ_WRITE, and
     * MEMORY_OPTION_DEVICE_READ_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_WRITE_ONLY = (1U << 13),

    /**
     * Memory option used for declaring that the requested memory will only be
     * accessed by the host. This option is mutually exclusive with the 
     * MEMORY_OPTION_DEVICE_READ_WRITE, MEMORY_OPTION_DEVICE_READ_ONLY, and
     * MEMORY_OPTION_DEVICE_WRITE_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_NO_ACCESS = (1U << 14),

    /**
     * Memory option used for declaring that the requested memory will be both
     * read and written by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_NO_ACCESS, MEMORY_OPTION_HOST_READ_ONLY, and
     * MEMORY_OPTION_HOST_WRITE_ONLY options. 
     */
    MEMORY_OPTION_HOST_READ_WRITE = (1U << 15),

    /**
     * Memory option used for declaring that the requested memory will only be
     * both read by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_NO_ACCESS, MEMORY_OPTION_HOST_READ_WRITE, and
     * MEMORY_OPTION_HOST_WRITE_ONLY options. 
     */
    MEMORY_OPTION_HOST_READ_ONLY = (1U << 16),

    /**
     * Memory option used for declaring that the requested memory will only be
     * both written by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_NO_ACCESS, MEMORY_OPTION_HOST_READ_WRITE, and
     * MEMORY_OPTION_HOST_READ_ONLY options. 
     */
    MEMORY_OPTION_HOST_WRITE_ONLY = (1U << 17),

    /**
     * Memory option used for declaring that the requested memory will only be
     * accessed by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_READ_WRITE, MEMORY_OPTION_HOST_READ_ONLY, and
     * MEMORY_OPTION_HOST_WRITE_ONLY options. 
     */
    MEMORY_OPTION_HOST_NO_ACCESS = (1U << 18),
} MemoryOption;

/**
 * @brief hsacommon ASICInfo class, represents ASIC specific information of a GPU device.
 */
enum ASICType
{
    ASIC_TYPE_SI_TAHITI,
    ASIC_TYPE_SI_PITCAIRN,
    ASIC_TYPE_CI_SKU1,
    ASIC_TYPE_CI_SKU2
};

typedef enum
{
    DEVICE_TYPE_CPU=AMD_DEVICE_TYPE_CPU,
    DEVICE_TYPE_GPU=AMD_DEVICE_TYPE_GPU,
    DEVICE_TYPE_INVALID=AMD_DEVICE_TYPE_UNKNOWN
} DeviceType;

class Exception;
static void *malloc_check(size_t n)
{
    void *vp = malloc(n);
    if (vp == NULL){
        // throw exception
        throw std::bad_alloc();
    }
    return vp;
}

static void *realloc_check(void * ptr, size_t size)
{
    void *vp = realloc(ptr, size);
    if (vp == NULL){
        // throw exception
        throw std::bad_alloc();
    }
    return vp;
}

static char * str_dup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *new_s = (char *)malloc_check(sizeof(char)*len);
    memcpy(new_s, s, len);
    return new_s;
}

class string{
private:
    char *cptr;
public:

    /**
     * @brief Default constructor, initializes empty string.
     */
    string()
        :cptr(str_dup(""))
    {
    }

    /**
     * @brief Copy constructor
     * @param str string to be copied for the new object
     */
    string (const string& str)
        :cptr(str_dup(str.c_str()))
    {
    }

    /**
     * @brief Copy constructor
     * @param s a null-terminated character sequence (C string)
     *        to be copied for the new object
     */
    string (const char *s)
        :cptr(str_dup(s))
    {
    }

    /**
     * @brief Copy constructor
     * @param s array of characters to be copied
     * @param n first n characters in the array will be copied
     * @note if s is null terminated and shorter then n,
     *       it's the same as string (const char *s)
     */
    string (const char *s, size_t n)
    {
        if (n <= strlen(s)){
            cptr = (char *)malloc_check(sizeof(char)*(n + 1));
            memcpy(cptr, s, n);
            cptr[n] = '\0';
        } else {
            cptr = str_dup(s);
        }
    }

    /**
     * @brief assignment operator function, assigns a copy of
     *        s as content of string object
     * @return returns *this, string object with copied content.
     * @param s a pointer to an array containing ac string,
     *          which is copied as the new content for the object.
     */
    string& operator= (const char* s)
    {
        if (cptr != s){
            char *np = str_dup(s);
            free(cptr);
            cptr = np;
        }
        return *this;
    }

    /**
     * @brief assignment operator function, assigns a copy of
     *        str as content of string object
     * @return returns *this, string object with copied content.
     * @param str a string to be copied  as the new content for the object.
     */
    string& operator= (const string& str)
    {
        return operator=(str.c_str());
    }

    /**
     * @brief assignment operator function, assigns a copy of
     *        c as content of string object
     * @return returns *this, string object with copied content.
     * @param c a character to be copied  as the new content for the object.
     */
    string& operator= (char c)
    {
        char * np = (char *)malloc_check(sizeof(char)*2);
        np[0] = c;
        np[1] = '\0';
        free(cptr);
        cptr = np;
        return *this;
    }

    /**
     * @brief Function returns the number of characters in the string.
     * @return returns the number of characters contained in the string.
     */
    size_t size() const
    {
        return strlen(cptr);
    }

    /**
     * @brief Function returns the number of characters in the string.
     * @return returns the number of characters contained in the string.
     */
    size_t length() const
    {
        return size();
    }

    /**
     * @brief Function empties the string
     */
    void clear()
    {
        cptr[0] = '\0';
    }

    /**
     * @brief Function returns true if the string is empty.
     * @return returns true only if the string is empty.
     */
    bool empty() const
    {
        return *cptr == '\0';
    }

    /**
     * @brief Function returns a c-string with the same content as the string object
     * @return a c-string with the same content as the string object
     */
    const char* c_str() const
    {
        return cptr;
    }

    /**
     * @brief Appends a copy of the argument to the string.
     * @return  returns *this, string object with appended content.
     * @param str a string to be appended to the string object
     */
    string& operator+= (const string& str)
    {
        return operator+=(str.c_str());
    }

    /**
     * @brief Appends a copy of the argument to the string.
     * @return  returns *this, string object with appended content.
     * @param s a c-string appended to the string object
     */
    string& operator+= (const char* s)
    {
        size_t cpy_len = strlen(s) + 1;
        size_t ori_len = strlen(cptr);
        cptr = (char *)realloc_check(cptr, ori_len + cpy_len);
        memmove(cptr + ori_len , s, cpy_len);
        return *this;
    }

    /**
     * @brief Appends a copy of the argument to the string.
     * @return  returns *this, string object with appended content.
     * @param c a charactor to be appended to the string object
     */
    string& operator+= (char c)
    {
        size_t ori_len = strlen(cptr);
        cptr = (char *)realloc_check(cptr, ori_len + 2);
        cptr[ori_len] = c;
        cptr[ori_len + 1] = '\0';
        return *this;
    }

    /**
     * @brief Appends a copy of the argument to the string.
     * @param c a charactor to be appended to the string object
     */
    void push_back (char c)
    {
        operator+= (c);
    }

    /**
     * @brief Appends a copy of the argument to the string.
     * @return  returns *this, string object with appended content.
     * @param str a string to be appended to the string object
     */
    string& append (const string& str)
    {
        return operator+=(str);
    }

    /**
     * @brief Appends a copy of the argument to the string.
     * @return  returns *this, string object with appended content.
     * @param s a c-string appended to the string object
     */
    string& append (const char* s)
    {
        return operator+=(s);
    }

    /**
     * @brief Constant function returns a constant reference to the character
     *        at index pos in the string.
     * @return returns a constant reference to the charactor at index
     *        pos in the string.
     * @param pos The index of the charactor being returned.
     */
    const char& operator[] (size_t pos) const
    {
        return cptr[pos];
    }

    /**
     * @brief Function returns the reference to the character
     *        at index pos in the string.
     * @return returns the reference to the charactor at index
     *        pos in the string.
     * @param pos The index of the charactor being returned.
     */
    char& operator[] (size_t pos)
    {
        return cptr[pos];
    }

    /**
     * @brief Constant function returns a constant reference to the character
     *        at index pos in the string.
     * @return returns a constant reference to the charactor at index
     *        pos in the string.
     * @throw Throw out_of_range exception when index pos is greater than the string size.
     * @param pos The index of the charactor being returned.
     */
    const char& at (size_t pos) const
    {
        if (pos >= size()){
            throw std::out_of_range("hsacommon::string::at() called on element outside range.");
        }
        return cptr[pos];
    }

    /**
     * @brief Function returns a reference to the character
     *        at index pos in the string.
     * @return returns a reference to the charactor at index
     *        pos in the string.
     * @throw Throw out_of_range exception when index pos is greater than the string size.
     * @param pos The index of the charactor being returned.
     */
    char& at (size_t pos)
    {
        if (pos >= size()){
            throw std::out_of_range("hsacommon::string::at() called on element outside range.");
        }
        return cptr[pos];
    }

    /**
     * @brief Compares the content of this object to the content of argument
     * @return 0 if the compared characters sequences are equal,positive sign
     *         if considered greater than the comparing string passed as parameter
     *         or negative sign if considered smaller than the comparing string passed as parameter
     * @param str string object with the content to be used as comparing string.
     */
    int compare (const string& str) const
    {
        return compare(str.c_str());
    }

    /**
     * @brief Compares the content of this object to the content of argument
     * @return 0 if the compared characters sequences are equal,positive sign
     *         if considered greater than the comparing string passed as parameter
     *         or negative sign if considered smaller than the comparing string passed as parameter
     * @param s c-string with the content to be used as comparing string.
     */
    int compare (const char* s) const
    {
        return strcmp(cptr, s);
    }

    /**
     * @brief Swaps the contents of the string with those of string object str
     * @param str a string object to swap its contents with those of the object.
     */
    void swap (string& str)
    {
        std::swap(cptr, str.cptr);
    }

    /**
     * @brief Destructor, free memory.
     */
    ~string()
    {
        free(cptr);
    }

};

/**
 * @brief Inserts the string object str into the output stream os.
 * @param os ostream object on which the insertion operation is performed.
 * @param str string object output by the operation.
 * @return The same output stream as parameter os.
 */
inline std::ostream & operator<<(std::ostream & os, const string & str)
{
    return os << str.c_str();
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator== (const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) == 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator== (const char* lhs, const string& rhs)
{
    return rhs.compare(lhs) == 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator== (const string& lhs, const char* rhs)
{
    return lhs.compare(rhs) == 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator!= (const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) != 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator!= (const char* lhs, const string& rhs)
{
    return rhs.compare(lhs) != 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator!= (const string& lhs, const char* rhs)
{
    return lhs.compare(rhs) != 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator< (const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) < 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator< (const char* lhs, const string& rhs)
{
    return rhs.compare(lhs) > 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator< (const string& lhs, const char* rhs)
{
    return lhs.compare(rhs) < 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator> (const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) > 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator> (const char* lhs, const string& rhs)
{
    return rhs.compare(lhs) < 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator> (const string& lhs, const char* rhs)
{
    return lhs.compare(rhs) > 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator<= (const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) <= 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator<= (const char* lhs, const string& rhs)
{
    return rhs.compare(lhs) >= 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator<= (const string& lhs, const char* rhs)
{
    return lhs.compare(rhs) <= 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator>= (const string& lhs, const string& rhs)
{
    return lhs.compare(rhs) >= 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator>= (const char* lhs, const string& rhs)
{
    return rhs.compare(lhs) <= 0;
}

/**
 * @brief Perform the comparison operation, between lhs and rhs.
 * @return comparison results
 */
inline bool operator>= (const string& lhs, const char* rhs)
{
    return lhs.compare(rhs) >= 0;
}

/**
 * @brief Combine the content of lhs and those of rhs.
 * @return A string object with the content of both lhs and rhs.
 */
inline string operator+ (const string& lhs, const string& rhs)
{
    string result(lhs);
    result += rhs;
    return result;
}

/**
 * @brief Combine the content of lhs and those of rhs.
 * @return A string object with the content of both lhs and rhs.
 */
inline string operator+ (const char* lhs, const string& rhs)
{
    string result(lhs);
    result += rhs;
    return result;
}

/**
 * @brief Combine the content of lhs and those of rhs.
 * @return A string object with the content of both lhs and rhs.
 */
inline string operator+ (char lhs, const string& rhs)
{
    string result;
    result = lhs;
    result += rhs;
    return result;
}

/**
 * @brief Combine the content of lhs and those of rhs.
 * @return A string object with the content of both lhs and rhs.
 */
inline string operator+ (const string& lhs, const char* rhs)
{
    string result(lhs);
    result += rhs;
    return result;
}

/**
 * @brief Combine the content of lhs and those of rhs.
 * @return A string object with the content of both lhs and rhs.
 */
inline string operator+ (const string& lhs, char rhs)
{
    string result(lhs);
    result += rhs;
    return result;
}

/**
 * @brief Swaps the contents of the string objects lhs and rhs
 * @param lhs a string object to be swapped.
 * @param rhs the other string object to be swapped.
 */
inline void swap (string& lhs, string& rhs){
    lhs.swap(rhs);
}


/**
 *******************************************************************************
 * @brief HSA status codes
 *
 * @details This enumeration defines the possible status values returned by
 * HSA runtime interfaces and exceptions.
 *******************************************************************************
 */
typedef enum {
    /// Operation completed successfully
    STATUS_SUCCESS = 0,

    /// Operation timed out
    STATUS_TIMEDOUT = 1,

    /// Failed to complete the operation due to an invalid interface argument
    STATUS_INVALID_ARGUMENT = -1,

    /// Failed to complete the operation due to resource shortage
    STATUS_OUT_OF_RESOURCES = -2,

    /// Failed to complete the operation due to resource unavailability
    STATUS_RESOURCE_UNAVAILABLE = -3,

    /// Failed due to invalid resource
    STATUS_INVALID_RESOURCE = -4,

    /// Failed due to the operation being unsupported
    STATUS_UNSUPPORTED_OPERATION = -5,

    /// Failed due to the operation is invalid
    STATUS_INVALID_OPERATION = -6,

    /// Failed to build the HSAIL kernel executable
    STATUS_BUILD_FAILED = -7,

    /// Failed due to the functionality hasn't implemented yet
    STATUS_NOT_IMPLEMENTED = -8,

    // Failed due to the memory type being unsupported
    STATUS_UNSUPPORTED = -9,

    /// Not categorized yet!!!
    STATUS_UNCATEGORIZED = -15,
} Status;

/**
 * @brief Excption class. This function is used to derive more detailed exception classes.
 */
class Exception:public std::exception
{
    typedef struct{
        string msg;
        int errCode;
    }errInfo;
protected:

    vector< errInfo > info_;

private:

    Exception& operator = (const Exception&);

public:
    ~Exception() throw(){}

    /**
     * @brief append info to an exception, this version requires a type
     *
     * @param fname name of the function
     * @param erinfo detail, probably better to keep it under 40 chars
     * @param status exception Status
     */
    virtual void appendInfo(const char *fname, const char *erinfo, Status status)
    {
        string tmp;
        tmp.append(fname);
        tmp.append(":");
        tmp.append(erinfo);
        errInfo tmpInfo;
        tmpInfo.msg = tmp;
        tmpInfo.errCode = status;
        info_.push_back(tmpInfo);
    }

    /**
     * @brief lastest exception status
     *
     * @return latest exception status in info_
     */
    int errCode(){
        return info_.back().errCode;
    }

    /**
     * @brief the virtual function in std::exception implemented here
     *
     * @return latest string of exception
     */
    const char* what() const throw()
    {
        const char * c = info_.back().msg.c_str();
        return c;
    }

    /**
     * @brief Getter function to access expection stack
     *
     * @return exception stack in a vector: each element is msg followed  by exception status
     */
    vector< errInfo > getExceptionVector(){
        return info_;
    }

    /**
     * @brief Getter function to access expection stack
     *
     * @return whole stack of exception info in a string: exception status followed by msg
     */

    string getExceptionStack(){
        string exStack;
        char tmp[40];
        exStack = "Status\tmsg\n";
        for (size_t i = info_.size() - 1 ; i >= 0; --i)
        {
            sprintf(tmp, "%d", info_[i].errCode);
            exStack += tmp;
            exStack += "\t";
            exStack += info_[i].msg;
            exStack += "\n";
        }
        return exStack;
    }

};

/**
 *******************************************************************************
 * @brief Invalid argument exception type
 *******************************************************************************
 */
class InvalidArgument: public Exception {};

/**
 *******************************************************************************
 * @brief Resource failure exception type
 *******************************************************************************
 */
class ResourceFailure: public Exception {};

/**
 *******************************************************************************
 * @brief Build failure exception type
 *******************************************************************************
 */
class BuildFailure: public Exception {};

/**
 *******************************************************************************
 * @brief Unsupported operation failure
 *******************************************************************************
 */
class UnsupportedOperation: public Exception {};

// macro to throw base class exception: shoud we remove this in the future?
#define ERR_THROW_EX(errVal, errMsg, expVal)                        \
    if (errVal){                                                    \
        Exception exObj;                                            \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }

// macros for derived exception classses
#define ERR_THROW_INVALIDARGUMENT(errVal, errMsg, expVal)           \
    if (errVal){                                                    \
        InvalidArgument exObj;                                      \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }

#define ERR_THROW_RESOURCEFAILURE(errVal, errMsg, expVal)           \
    if (errVal){                                                    \
        ResourceFailure exObj;                                      \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }

#define ERR_THROW_BUILDFAILURE(errVal, errMsg, expVal)              \
    if (errVal){                                                    \
        BuildFailure exObj;                                         \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }

#define ERR_THROW_UNSUPPORTEDOPERATION(errVal, errMsg, expVal)      \
    if (errVal){                                                    \
        UnsupportedOperation exObj;                                 \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }


class DLL_PUBLIC ASICInfo
{
public:
    /*! Getter function to check for image support.
     * @return bool indicating image support.
    */
    virtual bool isImageSupport()=0;
    /*! Getter function to check for double precision support.
     * @return bool indicating double support.
    */
    virtual bool isDoublePrecision()=0;
    /*! Getter function to check the ASIC Type.
     * @return returns an ASICType enumeration.
    */
    virtual ASICType getASICType()=0;
    virtual ~ASICInfo() {};
};

/**
 * @brief MemoryDescriptor class, each descriptor
 * provides interfaces to get different memory properties.
 */
class DLL_PUBLIC MemoryDescriptor
{
public:
    /**
     * @brief Getter function for MemoryType
     * @return returns a MemoryType enumeration.
     */
    virtual MemoryType getMemoryType()=0 ;
    /**
     * @brief Getter function for HeapType
     * @return returns a HeapType enumeration.
     */
    virtual HeapType getHeapType()=0;
    /**
     * @brief Getter function for lower 32 bits of memory size.
     * @return returns the lower 32 bits of memory size.
     * (example: if memory size = 4GB (2^32), getSizeLow() will
     *  return 0 since 2^32 = 0x1 00 00 00 00
     */
    virtual uint32_t getSizeLow()=0;
    /**
     * @brief Getter function for higher 32 bits of memory size.
     * @return returns the higher 32 bits of memory size.
     * (example: if memory size = 4GB (2^32), getSizeHigh() will
     *  return 1 since 2^32 = 0x1 00 00 00 00
     */
    virtual uint32_t getSizeHigh()=0;
    /**
     * @brief Getter function for memory size.
     * @return returns 64 bit integer representing memory size.
     */
    virtual uint64_t getSize()=0;
    /**
     * @brief Getter function for memory width (the number of parallel bits to
     *        the memory interface)
     * @return returns memory width.
     */
    virtual uint32_t getWidth()=0;
    /**
     * @brief Getter function for maximum memory clock which allows for
     *        computation of available bandwidth.
     * @return returns max memory clock.
     */
    virtual uint32_t getMaxMemoryClock()=0;

    virtual ~MemoryDescriptor() {};
};

/**
 * @brief CacheDescriptor class, each descriptor
 * provides interfaces to get different cache properties.
 */
class DLL_PUBLIC CacheDescriptor
{
public:
    enum CacheType { DATA=1, INSTRUCTION, CPU, SIMD };
     /**
     * @brief Getter function for ProcessorId
     * @return returns a processor number
     */
    virtual uint32_t    getProcessorId()=0;
    /**
     * @brief Getter function to query the cache level.
     * @return returns a number (1,2,3.. etc) representing the
     *         L1,L2,L3... cache levels.
     */
    virtual uint32_t    getCacheLevel()=0;
    /**
     * @brief Getter function for cache size
     * @return returns the size of the cache.
     */
    virtual uint32_t    getCacheSize()=0;
    /**
     * @brief Getter function for cache line size
     * @return returns the size of the cache line in bytes.
     */
    virtual uint32_t    getCacheLineSize()=0;
    /**
     * @brief Getter function for cache lines per tag.
     * @return returns the number of cache lines per tag.
     */
    virtual uint32_t    getCacheLinesPerTag()=0;
    /**
     * @brief Getter function for cache associativity.
     * @return returns the associativity of the cache.
     */
    virtual uint32_t    getCacheAssociativity()=0;
    /**
     * @brief Getter function for cache latency.
     * @return returns the cache latency in nano seconds.
     */
    virtual uint32_t    getCacheLatency()=0;
    /**
     * @brief Getter function for cache type
     * @return returns a CacheType enumeration
     */
    virtual CacheType    getCacheType()=0;
    virtual uint32_t    getSiblingMap()=0;
    virtual ~CacheDescriptor() {};
};

/**
 * @brief IOLinkDescriptor class, each descriptor
 * provides interfaces to get different iolink properties.
 */
class DLL_PUBLIC IOLinkDescriptor
{
public:
    enum IOLinkType { UNDEFINED=0, HYPERTRANSPORT, PCIEXPRESS, AMBA, OTHER };
    /**
     * @brief Getter function for IO Link Type.
     * @return returns a IoLinkType enumeration
     */
    virtual IOLinkType getIoLinkType()=0;
    /**
     * @brief Getter function for IO Bus interface Major version.
     * @return returns the major version of IO bus interface.
     */
    virtual uint32_t getMajorVersion()=0;
    /**
     * @brief Getter function for IO Bus interface Minor version.
     * @return returns the minor version of IO bus interface.
     */
    virtual uint32_t getMinorVersion()=0;
    /**
     * @brief Getter function for source node of an IOLink.
     * @return returns the source node Id of an IOLink.
     */
    virtual uint32_t getFromNode()=0;
    /**
     * @brief Getter function for destination node of an IOLink.
     * @return returns the destination node Id of an IOLink.
     */
    virtual uint32_t getToNode()=0;
     /**
     * @brief Getter function for weight associated with a link.
     * @return returns the weight associated to an IOLink.
     */
    virtual uint32_t getWeight()=0;
     /**
     * @brief Getter function for minimum transfer time.
     * @return returns the minimum cost of time to transfer in nano seconds.
     */
    virtual uint32_t getMinimumLatency()=0;
    /**
     * @brief Getter function for maximum transfer time.
     * @return returns the maximum cost of time to transfer in nano seconds.
     */
    virtual uint32_t getMaximumLatency()=0;
    /**
     * @brief Getter function for minimum IO interface bandwidth.
     * @return returns the minimum interface bandwidth in MB/s
     */
    virtual uint32_t getMinimumBandwidth()=0;
    /**
     * @brief Getter function for maximum IO interface bandwidth.
     * @return returns the maximum interface bandwidth in MB/s
     */
    virtual uint32_t getMaximumBandwidth()=0;
    /**
     * @brief Getter function for recommendedTxfrSize to achieve peak bandwidth.
     * @return returns the recommended transfer size in bytes.
     */
    virtual uint32_t getRecommendedTransferSize()=0;
    virtual ~IOLinkDescriptor() {} ;
};

/**
 * @brief Version class.
 * Version is of the form major.minor.patch.
 * Rules for incrementing the major, minor, patch versions:
 * a) major: 
 *      i) incremented only when public APIs signatures
 *         are changed, IS NOT backward compatible with
 *         previous major versions.
 *     ii) Reset the minor version to 0.
 *    iii) Reset the patch version to 0.
 * b) minor: 
 *      i) incremented when new feature is added, needs
 *         to be backward compatible with previous minor 
 *         versions.
 *     ii) Reset the patch version to 0.
 * c) patch: 
 *      i) incremented only for bug fixes, no user visible
 *         API changes, needs to be backward and forward
 *         compatible with previous patch versions.
 */
class Version
{
    uint32_t major_;
    uint32_t minor_;
    uint32_t patch_;
public:
    Version(uint32_t mj, uint32_t mn, uint32_t pt) : major_(mj), minor_(mn), patch_(pt) {   }
    /**
     * @brief Get the current major version
     *
     * @return returns the current major version
     */
    uint32_t getMajorVersion() { return major_; }
     /**
     * @brief Get the current minor version
     *
     * @return returns current minor version
     */
    uint32_t getMinorVersion() { return minor_; }
     /**
     * @brief Get the current patch version
     *
     * @return returns current patch version
     */
    uint32_t getPatchVersion() { return patch_; }
};


} // end namespace hsacommon

namespace hsa
{

using namespace hsacommon;

using hsacommon::vector;
using hsacommon::string;
using hsacommon::Version;

/**
*******************************************************************************
* union KernelArgs
*
* @brief This union provides a container for kernel argument types.
*******************************************************************************
*/
union RTKernelArgs
{
    void* addr;             ///< pointer to a buffer
    int32_t s32value;       ///< signed 32 bit value
    uint32_t u32value;      ///< unsigned 32 bit value
    float fvalue;           ///< float value
    double dvalue;          ///< double value
    int64_t s64value;       ///< signed 64 bit value
    uint64_t u64value;      ///< unsigned 64 bit value
};
}
#endif
