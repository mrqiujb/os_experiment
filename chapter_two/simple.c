/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>


struct birthday
{
      int day;
      int month;
      int year;
      struct list_head list;
};
static LIST_HEAD(birthday_list); // static struct list_head birthday_list = { &(birthday_list), &(birthday_list) }
/* This function is called when the module is loaded. */
static int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       struct birthday *mybirth;
       //mybirth=kmalloc(sizeof(struct birthday),GFP_KERNEL);
       size_t i=2;
       for (; i < 8; i++)
       {
              mybirth=kmalloc(sizeof(struct birthday),GFP_KERNEL);
              mybirth->day=i;
              mybirth->month=i;
              mybirth->year=i;
              list_add_tail(&mybirth->list,&birthday_list);
       }
       struct birthday *p;
       list_for_each_entry(p,&birthday_list,list){
              printk("day: %d\n",p->day);
              printk("month: %d\n",p->month);
              printk("year: %d\n",p->year);
       }
       return 0;
}

/* This function is called when the module is removed. */
static void simple_exit(void) {
       struct birthday *p,*next;
       list_for_each_entry_safe(p,next,&birthday_list,list){
              printk("day %d is delete\n",p->day);
              list_del(&p->list);
              kfree(p);
       }
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

