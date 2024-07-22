[toc]
mysql数据库连接池C++实现


数据库连接池:
对于数据库操作都是在访问数据库的时候创建连接，访问完毕断开连接。但是如果在高并发情况下，有些需要频繁处理的操作就会消耗很多的资源和时间，比如：

建立通信连接的TCP三次握手
数据库服务器的连接认证
数据库服务器关闭连接时的资源回收
断开通信连接的TCP四次挥手
数据库连接池负责分配、管理和释放数据库连接，它允许应用程序重复使用一个现有的数据库连接，而不是再重新建立一个。在并发程度比较高的时候，连接池技术尽可能多地重用了消耗内存地资源，大大节省了内存，提高了服务器地服务效率，能够支持更多的客户服务。通过使用连接池，将大大提高程序运行效率。


---------------------华为实习面试总结------------------------------------------
这一点和线程池的思想很像。
线程池的一个目的是在一个不断需要创建线程执行分发任务的场景，如果不断创建、使用、析构，相当耗费资源。所以使用线程池，不需要重复创建、析构线程，而是不断向线程分配任务

当然数据库连接池也是一样，如果有一个(就像拍照亭这样)不断创建数据库连接执行数据库查询的任务(多线程)，此时如果不断创建数据库注册、连接、断开连接，这会不断init、tcp握手、tcp挥手，所以使用数据库连接池。允许应用程序重复使用一个现有的数据库连接。


应用 : 在mysql连接池中使用自定义shared_ptr删除函数来归还连接
MysqlConnPool.cpp中
```c++
shared_ptr<MysqlConn> connptr(m_connectionQ.front(), [this](MysqlConn* conn) {
    unique_lock<mutex> locker(m_mutexQ);
    conn->refreshAliveTime();
    conn->transaction(false);
    m_connectionQ.push(conn);
});
```
这里做一个解释 为什么传入删除函数的参数MysqlConn* conn就一定是当前shared_ptr中指向的对象：
在shared_ptr的构造中，创建了这么一个对象:
```c++
    //这里Dt就是我们传入的删除函数
    template <class _Ux, class _Dx,
        enable_if_t<conjunction_v<is_move_constructible<_Dx>, _Can_call_function_object<_Dx&, _Ux*&>,
                        _SP_convertible<_Ux, _Ty>>,
            int> = 0>
    shared_ptr(_Ux* _Px, _Dx _Dt) { // construct with _Px, deleter
        _Setpd(_Px, _STD move(_Dt));
    }
    template <class _UxptrOrNullptr, class _Dx>
    void _Setpd(const _UxptrOrNullptr _Px, _Dx _Dt) { // take ownership of _Px, deleter _Dt
        _Temporary_owner_del<_UxptrOrNullptr, _Dx> _Owner(_Px, _Dt);
        _Set_ptr_rep_and_enable_shared(
            _Owner._Ptr, new _Ref_count_resource<_UxptrOrNullptr, _Dx>(_Owner._Ptr, _STD move(_Dt)));
        _Owner._Call_deleter = false;
    }
    //对于_Temporary_owner_del _Owner对象 在给如Dt的时候析构函数如下 由此可以看出删除函数讲自身绑定指针传入给自定义的删除函数
    ~_Temporary_owner_del() {
        if (_Call_deleter) {
            _Dt(_Ptr);
        }
    }
    //所以这里的MysqlConn* conn就是shared_ptr管理的对象
```


