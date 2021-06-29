#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <sqlite3.h>


char *printf_md5(unsigned char *in,char c[])
{
    char s[100] = "";
    int i;
    char buffer[100];
    for(i = 0 ; i < MD5_DIGEST_LENGTH ; i++) {
        if ( in[i] < 0x10){
            printf("0%1x", in[i]);
            // char buffer1[4];
            sprintf(buffer,"%1x",in[i]);
            strcat(c,"0");
            strcat(c,buffer);
            }
        else{
            printf("%2x", in[i]);
            
            sprintf(buffer,"%2x",in[i]);
            strcat(c,buffer);
        }
    }
    char *a = s;
    printf("%s",a);
    printf("%s",s);
    printf("\n");
    
    return c;
    // printf("                                %s\n",s);

    
    
   
    
}
void md5(char *file_path,int chunk_size,char *db_name,char *table_name){
    size_t file_size = 0;
    FILE *fsrc = fopen(file_path, "rb");  // 源文件
    fseek(fsrc, 0, SEEK_END); /* Go to end of file */
    file_size = ftell(fsrc); /* How many bytes did we pass ? */
    rewind( fsrc );
    printf("%zu\n",file_size);
    //--------------open database
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *err_msg = 0;
    rc = sqlite3_open(db_name, &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    //----------------
    unsigned char buffer[chunk_size];
    MD5_CTX context;
    MD5_Init(&context);
    int bytes;
    int idx = 0;
    while(   (bytes = fread(buffer,1,chunk_size,fsrc)   ) !=0    ){
        
        printf("offset:%d, size:%d, checksum: ",idx++,bytes);
        unsigned char digest[bytes];
        MD5_Update(&context, buffer, bytes);
        MD5_Final(digest, &context);
        char x[100]="";
        printf_md5(digest,x);
        
        char* query = sqlite3_mprintf("insert into '%s' values ('%d', '%s','%d');",table_name, idx,x,bytes);
        rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    
    } 
    sqlite3_close(db);

}
void create_table (char *db_name ,char *table_name){
    sqlite3 *db;
    int rc;
    char *err_msg = 0;
    rc = sqlite3_open(db_name, &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    printf("%s",table_name);
    char* query = sqlite3_mprintf("CREATE TABLE '%s'( "\
                                    "Id INT, "\
                                    "Name TEXT,"\
                                    "Price INT);" ,table_name);
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    sqlite3_close(db);
}
int main(int argc, char **argv)
{
    create_table("test3.db","chunk_list");
    md5(argv[1],4096,"test3.db","chunk_list");
    
    return 0;
}