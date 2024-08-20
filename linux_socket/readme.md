socket底层使用TCP实现，所以在传输数据的时候必选转换为字节流
例如此时需要传输一个自定义的class 就必须实现这个class的流化编码和反编码
```c++
class Person {
public:
    std::string name;
    int age;

    Person() : name(""), age(0) {}
    Person(const std::string& name, int age) : name(name), age(age) {}

    // 序列化
    size_t serialize(char* buffer) const {
        size_t name_length = name.size();
        memcpy(buffer, &name_length, sizeof(name_length));
        buffer += sizeof(name_length);
        memcpy(buffer, name.c_str(), name_length);
        buffer += name_length;
        memcpy(buffer, &age, sizeof(age));
        return sizeof(name_length) + name_length + sizeof(age);
    }

    // 反序列化
    static Person deserialize(const char* buffer) {
        size_t name_length;
        memcpy(&name_length, buffer, sizeof(name_length));
        buffer += sizeof(name_length);

        std::string name(buffer, name_length);
        buffer += name_length;

        int age;
        memcpy(&age, buffer, sizeof(age));

        return Person(name, age);
    }
};
```

服务器端在传输前
```c++
Person replyPerson("Server", 99);
size_t dataSize = replyPerson.serialize(buffer);
send(new_socket, buffer, dataSize, 0);
```
客户端接受后
```c++
int valread = read(sock, buffer, BUFFER_SIZE);
if (valread > 0) {
    Person receivedPerson = Person::deserialize(buffer);
    std::cout << "Received person from server: " << receivedPerson.name << ", " << receivedPerson.age << std::endl;
}
```


# 服务端和客户端是怎么设置的呢
服务端设置port为8000 ip为INADDR_ANY,表示监听任意ip端口为8000的socket连接
服务端经过创建socket、bind、listen后如果有客户端传入就会使用accept接收一个新的客户端连接 

客户端只需要创建socket后根据ip、port直接connect即可