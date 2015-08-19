#ifndef _QUEUESEND_
#define _QUEUESEND_

#include <iostream>

using namespace std;

//Queue的实现类  
//这里是特定的模板友元关系，即类可以只授权特定的实例的访问权，即类Queue和类QueueItem是一对一的关系  
template<class T> class Queue;  
template <class T>  
class QueueItem  
{  
    //因为Queue是需要使用到这个不公开接口的类中的成员,所以需要将Queue声明为友元类  
    friend class Queue<T>;  
private:  
    //复制构造函数  
    QueueItem<T>( const T &q ): item( q ) , next( 0 )  {}  
      
      
    //元素值  
    T item;  
      
    //指向下一个元素的指针  
    QueueItem<T> *next;  
};  
  
//类模板  
template <class T>  
class Queue  
{  
public:  
    //需要注意的一点是,一般在类中实现的函数都会是内联函数  
    //使用内联函数的要求是,该函数的代码一般只有几行,并且经常执行  
      
      
      
    //默认构造函数  
      
    //这样的声明也是可以的  
    Queue<T>() :head( 0 ) , tail( 0 ) {}  
      
      
      
    //复制构造函数  
      
    //当然这样的声明也是可以的  
    //Queue( const Queue &q ): head( q.head ) , tail( q.tail ) { copy_elems( q ); }  
    Queue<T>( const Queue<T> &q) :head( q.head ) , tail( q.tail ) { copy_elem( q ); }  
      
      
      
    //操作符重载  
    Queue<T>& operator= ( const Queue<T>& );  
      
      
    //析构函数  
    ~Queue(){ destroy(); }    
      
      
    //获取头节点的元素  
    T& front() { return head->item; }  
      
      
    //入队列  
    void push( const T& );  
      
    //出队列  
    void pop();  
      
    //判断是否为空  
    bool empty() const { return NULL == head; }  
      
      
    //显示队列元素  
    void ShowElements() ;  
      
      
      
private:  
    //队列的头指针,尾指针,主要用于出入队列用  
    QueueItem<T> *head;  
    QueueItem<T> *tail;  
      
    //销毁队列数据  
    void destroy();  
      
    //复制元素  
    void copy_elems( const Queue& );  
};  
  
//出队列,即删除队列头部的元素  
template<typename T>   
void Queue<T>::pop()  
{     
    //判断是否为空指针  
    if( NULL == head )  
    {  
        return ;  
    }  
      
    //保存当前指针的值  
    QueueItem<T> *p = head;  
      
    //将头指针指向下一个元素  
    head = head->next;  
      
    //删除  
    delete p;  
}  
  
  
//入队列,即从队列的尾部插入数据  
template<typename T>  
void Queue<T>::push( const T &t )  
{  
    //构造一个对象  
    QueueItem<T> *p = new QueueItem<T>( t );  
      
      
    //在插入队列尾部的时候需要判断队列是否为空的！  
    if( empty() )  
    {  
        head = tail = p;  
    }  
    else  
    {  
        //将尾指针的指向下一个元素的指针指向生成的数据  
        tail->next = p;  
          
        //将尾指针移动到最后一个数据上去  
        tail = p;  
    }  
}  
  
  
  
//销毁数据  
template<typename T>  
void Queue<T>::destroy()  
{  
    //不断地出队列即可  
    while( !empty() )  
    {  
        pop();  
    }  
}  
  
  
//赋值操作符重载  
template<typename T>  
Queue<T>& Queue<T>::operator= (const Queue<T> &q)  
{  
    //复制队列元素,结束条件是head为空的时候  
    for( QueueItem<T> *p= q.head ; p  ; p = p->next )  
    {  
        push( p->item );  
    }  
      
}  
  
#endif