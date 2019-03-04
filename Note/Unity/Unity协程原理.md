# Unity协程原理

Unity中的协程实际上只是利用迭代器，在Monobehavior的生命周期中注册了一个函数，
然后每帧进行调用，针对不同的yield进行不同的处理。