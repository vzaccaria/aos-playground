

# Quick recap of Linux kernel double linked lists

Double linked lists in Linux are created by adding a `list_head` field in the structure of any element.
This structure contains two pointers: `next` and `prev`, which point to the next and previous elements in the list respectively.

Here's how you can define a doubly linked list in Linux:

```c
struct list_element {
    int data;
    struct list_head list;
};
```
In this case, `list_element` is a structure that has an integer variable `data` and a `list_head` variable `list`. The latter serves as an anchor point for the double linked list. 

You can manipulate (insert, delete, search) elements in this linked list using macros provided by Linux such as `list_add()`, `list_del()`, etc. For instance, if you want to add a new element to this list:

```c
struct list_element *new_node;
new_node = (struct list_element *)kmalloc(sizeof(struct list_element), GFP_KERNEL);
new_node->data = 100;

INIT_LIST_HEAD(&new_node->list);
list_add(&new_node->list, &mylist);
```
In the above code snippet, we create a new node (`new_node`), initialize its `list` field, and then add it to an existing list (`myList`). This is how the doubly linked lists in Linux are created and manipulated.

The use of macros allows these operations on the lists to be somewhat polymorphic - meaning they can work with any type of data as long as it has the required `list_head` field.


![](Images/readme%202023-11-04%2016.14.40.excalidraw.png)

# Module description
This Kernel module demonstrates two variants of reader/writer threads manipulating a shared list.  The initialisation code fills up a shared list with initial data,
creates and starts reader and writer kthreads. 

The `manipulate_list_thread_*()` kthread, manipulates the shared list by removing the first element if any, incrementing its value and adding it back. `read_list_thread_*` just prints the current list.

## Variant 1
Variant 1 of the reader-writer threads (`read_list_thread_norcu` and `manipulate_list_thread_norcu`) lacks proper synchronization, which may result in list corruption. Just check by running it!

## Variant 2
Variant 2  of of the reader-writer threads (`read_list_thread_rcu` and `manipulate_list_thread_rcu`) employes Read-Copy-Update (RCU) synchronization to avoid such issues.

--- 

Diagrams:
%%[🖋 Edit in Excalidraw](Images/readme%202023-11-04%2016.14.40.excalidraw.md), and the [dark exported image](Images/readme%202023-11-04%2016.14.40.excalidraw.dark.png)%%
