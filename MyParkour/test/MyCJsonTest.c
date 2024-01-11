//
// Created by Ignorant on 2023/12/20.
//


#include <stdio.h>
#include <string.h>
#include "../include/cJSON.h"

/*
 * cJson的定义:
 * typedef struct cJSON {
 *     // next/prev: 用于在数组或对象中遍历链表
 *     // 也可以使用 GetArraySize / GetArrayItem / GetObjectItem
 *     struct cJSON *next;
 *     struct cJSON *prev;
 *     // child: 如果当前项是一个数组或对象,child指向一个链表,表示数组或对象中的各个元素
 *     struct cJSON *child;
 *     // type: 表示当前项的类型,可以是下面几种:
 *     // cJSON_False / cJSON_True / cJSON_NULL / cJSON_Number / cJSON_String / cJSON_Array / cJSON_Object / cJSON_Raw
 *     int type;
 *     // 如果当前项类型是cJSON_String / cJSON_Raw,valuestring指向存储字符串值的字符数组
 *     char *valuestring;
 *     @Deprecated
 *     // valueint: 用于存储整数值,但推荐使用 cJSON_SetNumberValue 函数设置数字值
 *     int valueint;
 *     // valuedouble: 如果当前项类型是cJSON_Number,valuedouble存储该项的数字值
 *     double valuedouble;
 *     // string: 如果当前项是一个对象的成员,string存储该成员的 key; 如果当前项是一个数组的元素,string可能为NULL
 *     char *string;
 * } cJSON;
 */

int main() {
    // 创建json对象
    // 1.创建对象: CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void)
    cJSON *root1 = cJSON_CreateObject();
    // 2.创建键值对: CJSON_PUBLIC(cJSON*) cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string1)
    cJSON *key1 = cJSON_AddStringToObject(root1, "key1", "10");
    // 获取 key 和 value
    if (key1 != NULL) {
        printf("key: %s; value: %s\n", key1->string, key1->valuestring);
    }
    // 3.嵌套json对象: CJSON_PUBLIC(cJSON_bool) cJSON_AddItemToObject(cJSON *object, const char *string1, cJSON *item)
    cJSON *last = root1;    // 上一个创建的obj对象
    for (int i = 0; i < 3; i++) {
        cJSON *obj = cJSON_CreateObject();
        cJSON_bool isSuccess = cJSON_AddItemToObject(last, "obj", obj);
        if (isSuccess) {
            printf("Success!\n");
        }
        last = obj;
    }
    /*
     * 4.添加数组:
     *  CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count);
     *  CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count);
     *  CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count);
     *  CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char *const *strings, int count);
     */
    int nums[] = {1, 2, 3, 4, 5};
    cJSON *array = cJSON_CreateIntArray(nums, 5);
    cJSON_AddItemToObject(root1, "array", array);
    // 5.输出json对象内容: CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item)
    char *string1 = cJSON_Print(root1);
    printf("%s\n", string1);
    // 6.将json对象写成文件
    FILE *file = fopen("test.json", "w");
    if (file != NULL) {
        const char *buffer = cJSON_Print(root1);
        fwrite(buffer, strlen(buffer), 1, file);
        fclose(file);
    }
    // 7.释放内存: CJSON_PUBLIC(void) cJSON_Delete(cJSON *item)
    cJSON_Delete(root1);
    
    // 解析json对象
    // 1.将json文件转换为字符串
    FILE *file2 = fopen("test.json", "r");
    if (file2 != NULL) {
        fseek(file2, 0, SEEK_END);
        int fileSize = ftell(file2);
        rewind(file2);
        char *string2 = cJSON_malloc(fileSize * sizeof(*string2));
        memset(string2, 0, fileSize);
        fread(string2, sizeof(*string2), fileSize, file2);
        printf("%s\n", string2);
        // 2.创建json对象
        cJSON *root2 = cJSON_Parse(string2);
        // 3.获取键值内容
        // 数字
        const cJSON *node1 = cJSON_GetObjectItem(root2, "key1");
        if (node1 != NULL) {
            printf("key: %s\n", node1->valuestring);
        }
        // 数组
        const cJSON *node2 = cJSON_GetObjectItem(root2, "array");
        if (node2 != NULL) {
            int len = cJSON_GetArraySize(node2);
            for (int i = 0; i < len; i++) {
                const cJSON *item = cJSON_GetArrayItem(node2, i);
                printf("%.2f ", item->valuedouble);
            }
        }
        cJSON_Delete(root2);
        cJSON_free(string2);
        fclose(file2);
    }
    return 0;
}
