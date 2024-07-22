#include<iostream>
#include<algorithm>
#include<cstring>

class my_String {
private:
    char* _data;
    size_t _len;
public:
    my_String() : _len(0) {
        _data = new char[1];
        *_data = '\0';
    };
    my_String(const char* str) {
        if (str == nullptr) {
            _data = new char[1];
            *_data = '\0';
            _len = 1;
        }
        else{
            _data = new char[strlen(str) + 1];
            strcpy(_data, str);
            _len = strlen(str);
        }
    }
    my_String(const my_String& other) {
        if (*other._data == '\0') {
            _data = new char[1];
            *_data = '\0';
            _len = 0;
        }
        else {
            _data = new char[other.size() + 1];
            strcpy(_data, other.get());
            _len = other.size();
        }
    }
    my_String(my_String&& other) {
        _data = other.get();
        _len = other.size();
        other.clear();
    }
    my_String& operator = (const my_String& other) {
        if (&other == this) {
            return *this;
        }
        _data = new char[other.size() + 1];
        strcpy(_data, other.get());
        _len = other.size();
    }
    inline size_t size() const {
        return _len;
    }
    inline size_t length() const {
        return _len;
    }
    inline char* get() const {
        return _data;
    }
    //clear不能delete数据 因为移动构造如果这里delete了
    void clear() {
        _data = nullptr;
        _len = 0;
    }
    ~my_String() {
        delete[] _data;
        _data = nullptr;
        _len = 0;
    }
    my_String operator+(const my_String& other) const {
        size_t new_len = _len + other._len;
        char* new_data = new char[new_len + 1];
        strcpy(new_data, _data);
        if(other.get() != nullptr){
                strcat(new_data, other.get());
        }
        my_String result(new_data);
        delete[] new_data;
        return result;
    }
    my_String operator+=(const char ch) {
        size_t add_len = 1;
        char* new_data = new char[_len + add_len+1];
        if (_data != nullptr) {
            strcpy(new_data, _data);
            delete[] _data;
            _data = nullptr;
        }
        new_data[_len] = ch;
        new_data[_len+1] = '\0';
    
        _data = new_data;
        _len += add_len;
        return *this;
    }
    my_String operator+=(const char* str) {
        size_t add_len = strlen(str);
        char* new_data = new char[_len + add_len+1];
        if (_data != nullptr) {
            strcpy(new_data, _data);
            delete[] _data;
            _data = nullptr;
        }
        strcpy(new_data+_len,str);
        _data = new_data;
        _len += add_len;
        return *this;
    }
    my_String& operator+=(const my_String& other) {
        if (&other == this) {
            return *this;
        }
        size_t other_len = other.size();
        size_t old_len = _len;
        size_t add_len = other_len + old_len;
        _len = add_len;
        char* new_data = new char[_len + 1];
        if (_data != nullptr) {
            strcpy(new_data, _data);
            delete[] _data;
            _data = nullptr;
        }
        if (other._data != nullptr) {
            strcpy(new_data + old_len, other._data);
            //delete[] other._data;
            //other._data = nullptr;
        }
        _data = new_data;
        return *this;
    }
    void show_char() {
        if(_data==nullptr){
            std::cout <<"empty" <<std::endl;
            return;
        }
        std::cout <<"size is " << _len <<" string is :"<< _data << std::endl;
    }
    friend std::ostream& operator<<(std::ostream & os, my_String& str);
};

std::ostream& operator <<(std::ostream& os, my_String& str) {
    os << str.get();
    return os;
}
void my_str() {
/*
    自己实现string
    不考虑迭代器和地址分配器
*/
    my_String s1 = "hellow my_string";
    s1.show_char();
    my_String s2(s1);
    s2.show_char();
    my_String s3(std::move(s2));
    s3.show_char();

    std::cout << "after move create s2 is" << std::endl;
    s2.show_char();

    my_String s4 = s1 + s2;
    s4.show_char();
    s4 += "!!abc__hellow my_string";
    s4.show_char();
    s4 += s1;
    s4.show_char();
    s4+=s2;
    s4.show_char();
    s4+='a';
    s4.show_char();

    my_String s5;
    s5 += s4;
    s5.show_char();
}
int main(){
    my_str();
    return 0;
}