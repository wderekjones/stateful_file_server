int i,j,k;
int fd1,fd2,fd3;
char buffer[100];
fd1=Open("File1"); // opens the file
for (i=0; i< 20;i++){
Write(fd1, "funny contents in the file 1", 15);
}
Close(fd1);
fd2=Open("File1");
for (j=0; j< 20;j++){
Read(fd2, buffer, 10);
printf("%s\n",buffer);
}
Close(fd2);
Delete("File1");
List();
