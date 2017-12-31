#include <string.h>
#include "C0Compiler.h"
int  array[50] = {0};     //分程序索引数组
int  i = 0;               //分程序索引指针

f = 0;                    //记录主函数的个数
index = 0;                //符号表指针，符号表中元组个数
retflag = 0;              //记录Int类型函数的返回情况
//填符号表
void inserttable(char name[],int kind,int value,int address,int paranum,int indexs)
{
 int j;
 //符号表已满
 if (indexs>=200) {
  error(3);          
  exit(0) ;
 }
 //如果插入的是子程序
 if (kind==3) {
  //比较有没有函数名重定义，i表示当前的符号表的符号个数
  j = 1;
  while (j<=i) {
   if (strcmp(table[array[j]].name,name)==0) {
    error(4);
    return;
   }
   j++;
  }
 }
 //如果插入的是常量或者变量
 else {
  //检查有无重名的标志符，j为当前函数在符号表中的第一个位置
  j = array[i];                             
  while (j<indexs) {
   if (strcmp(table[j].name,name)==0) {
    error(4);
    return;
   }
   else j++;
  }
  j=0;
  if (kind!=4)
   while (j<array[1]) {
    if (strcmp(table[j].name,name)==0) {
     error(4);
     return;
    }
    else j++;
   }
 }
 
 //填写符号表
 strcpy(table[indexs].name,name);
 table[indexs].kind = kind;
 table[indexs].value = value;
 table[indexs].address = address;
 table[indexs].paranum = paranum;
 //管理分程序索引表，如果是函数，那么就要建立一个函数的符号表索引，这里注意，只有插入的是一个函数的时候i才会++
 if(kind==3) array[++i] = indexs;
 //所以array数组永远都存放的是符号表中函数标识符在符号表中的index值
 
 index++;
}
//向子程序项插入参数个数
void insertproc(int paranum)
{
 int x = array[i];   //array[i]，指向的是当前正在被分析的函数在符号表中的索引
 table[x].paranum = paranum;//插入函数参数个数信息
 return ;
}
//查符号表，返回负数，表示出现问题
int lookup(char name[],int flag)       //flag=1时，说明要在分程序索引里查找，flag=0时，在当前所在分程序符号表中查找
{
 int n;
 int m = array[1];
 if (flag==1) {
  n = 1;
  while (n<=i) {
   if(strcmp(table[array[n]].name,name)==0) break;
   n++;
  }
  //如果函数标志符没有定义
  if (n>i) {
   error(19);            
   printf("%s!",name);
   return -1;
  }
  //如果函数标识符无内容
  if (table[array[n]].value==0) return -2;
  //如果函数的实参个数与形参个数不匹配
  if (table[array[n]].paranum!=paranums) {
   error(21);                
   return -3;
  }
  return table[array[n]].address;
 }
 else {
  n = array[i];
  //查找符号表中是否有
  while (n<index) {
   if (strcmp(table[n].name,name)==0) return;
   n++;
  }
  //如果分程序符号表中无定义，查找最外层符号表
  if (n==index) { 
   n = 0;
   while (n<m) {
    if(strcmp(table[n].name,name)==0) return;
    n++;
   }
   //符号表中没有定义
   if (n==m) {
    error(19);
    printf("%s!",name);
    return -1;
   }
  }
  if(table[n].kind==11 || table[n].kind==12 || table[n].kind==13) return table[n].address;
//  if(table[n].kind==21 || table[n].kind==22 || table[n].kind==13) return -2;      //常量标志
  if(table[n].kind==4) return table[n].address;   //参数标志
  return -1;
 }
}
//清空符号表
void clean()
{
 int x = index-1;
 int y;
 while( (table[x].kind==11 || table[x].kind==12 || table[x].kind==13 ||table[x].kind==21
     || table[x].kind==22 || table[x].kind==23 || table[x].kind==4) && strcmp(table[x].name,procname)!=0) {
  table[x].kind = 0;
  table[x].address = 0;
  table[x].paranum = 0;
  table[x].value = 0;
  y = 0;
  while (y<30)
   table[x].name[y++] = '\0';
  x--;
 }
 index = x+1;
 return;
}
//＜程序＞  ::= ［＜常量说明部分＞］［＜变量说明部分＞］{＜有返回值函数定义部分＞|＜无返回值函数定义部分＞}＜主函数＞
void procedure()
{
 getsym();
 
 //如果出现const，则是＜常量说明部分＞
 if (sym==constsym) {
  //调用常量说明子程序
  conststate();
  //如果是分号,则是正常的
  while (sym_temp==semsym) {
   if (sym==constsym)
    conststate();
   else break;
  }
 }
 //变量定义和有返回值的函数是相同的声明头部。因此预读3个单词用于判断
 while (sym==intsym || sym==floatsym || sym==charsym) {
  symkind temp = sym;
  ch_temp = ch;
  codec_temp = codec;
  if (sym==intsym) kind = 11;
  else if (sym==floatsym) kind = 12;
  else if (sym==charsym) kind = 13;
  getsym();
  strcpy(name,id);//拷贝名字
  exchange(name);//大小写转换
  //如果标识符合法
  if (sym==identitysym) {
   getsym();
   //如果是逗号或者分号，则是变量定义
   if (sym==commasym) {
    sym = temp;
    codec = codec_temp;
    ch = ch_temp;
    varstate();
    continue;
   }
   else if (sym==semsym) {
    value = 0;
    address = 0;
    paranum = 0;
    inserttable(name,kind,value,address,paranum,index);
    if (kind==11) emit("int","  ","  ",name);
    else if (kind==12) emit("float","  ","  ",name);
    else if (kind==13) emit("char","  ","  ",name);
    getsym();
    continue;
   }
   else {
    sym = temp;
    codec = codec_temp;
    ch = ch_temp;
    break;
   }
  }
 }
 
 //进入函数部分
 while (sym==intsym || sym==floatsym || sym==charsym || sym==voidsym) {
  symkind temp = sym;
  //＜有返回值函数定义部分＞  ::=  ＜声明头部＞'('＜参数＞')'  '{'＜复合语句＞'}'
  if (sym==intsym || sym==floatsym || sym==charsym) {
   defhead();
   if (sym!=lparenthesessym) {
    error(8);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   n = 0;     //临时变量重新开始计数
      retflag = 0;//将是否存在返回值的标记初始化，0表示无返回值，即未出现return
      kind = 3;//表示当前的函数是子程序，即非主函数
      value = 1;//1对于函数来说，表示返回值为Int 0返回值为void
      address = 0;
      paranum = 0;
      inserttable(name,kind,value,address,paranum,index);
      strcpy(procname,name);//函数名拷贝
   if (temp==intsym) emit("start","int","  ",procname);
   if (temp==floatsym) emit("start","float","  ",procname);
   if (temp==charsym) emit("start","char","  ",procname);
   getsym();
   parametertable();
   //缺'('＜参数＞')'的右小括号
   if (sym!=rparenthesessym){
    error(9);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   //缺'{'＜复合语句＞'}'的左大括号
   if (sym!=lbracketsym) {
    error(10);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   complexsentence();
   //缺'{'＜复合语句＞'}'的右大括号
   if (sym!=rbracketsym) {
    error(11);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
    
   //函数缺少返回值
   if(retflag==0) {
    error(22);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   emit("end","  ","  ",procname);//函数部分四元式结束
   clean();//清楚符号表，这里注意该学长是先把变量生成了四元式！然后在生成函数定义的四元式，
   //但是符号便还是先函数定义再参数定义的   !!!!!!! 这里清空的是此子函数的符号表!!!!!!!
  }
  if (sym==voidsym) {
   getsym();
   //如果下一个单词是main，则是＜主函数＞    ::= void main '(' ')'  '{'＜复合语句＞'}'
   if (sym==mainsym) {
    n = 0;        //临时变量重新开始计数
    strcpy(name,"main");
    kind = 3;
    value = 0;
    address = 0;
    paranum = 0;
    inserttable(name,kind,value,address,paranum,index);
    strcpy(procname,name);
    emit("start","  ","  ",procname);
    getsym();
    if (sym!=lparenthesessym) {
     error(8);
     while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
      if (sym==EOFsym) exit(0);
      getsym();
     }
     continue;
    }
    getsym();
    if (sym!=rparenthesessym) {
     error(9);
     while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
      if (sym==EOFsym) exit(0);
      getsym();
     }
     continue;
    }
    getsym();
    if (sym!=lbracketsym) {
     error(10);
     while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
      if (sym==EOFsym) exit(0);
      getsym();
     }
     continue;
    }
    getsym();
    complexsentence();
    f++;    //主函数的个数+1
    emit("end","  ","  ",procname);
    return;    //源程序结束，跳出
   }
   //＜无返回值函数定义部分＞  ::= void＜标识符＞'('＜参数＞')'  '{'＜复合语句＞'}'
   if (sym!=identitysym) {
    error(1);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   n = 0;          //临时变量重新开始计数
   strcpy(name,id);
   exchange(name);
   kind = 3;
   value = 0;
   address = 0;
   paranum = 0;
   inserttable(name,kind,value,address,paranum,index);
   strcpy(procname,name);
   emit("start","void","  ",procname);
   getsym();
   if (sym!=lparenthesessym) {
    error(8);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   parametertable();
   if (sym!=rparenthesessym) {
    error(9);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   if (sym!=lbracketsym) {
    error(10);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   complexsentence();
   if (sym!=rbracketsym) {
    error(11);
    while (sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    continue;
   }
   getsym();
   emit("end","  ","  ",procname);
   clean();
  }
 }
}
void constdef( symkind per_sym )          
{
 if (sym==identitysym) {
  strcpy(name,id);//id即表示标识符的内容
  exchange(name);
  getsym();//读取下一个单词。应该为等号
  if (sym==becsym) {
   getsym();
   //如果等号后出现的第一个单词是+|-,则语法是正确的，定义的有符号的整数
   if (sym==addsym || sym==subtrationsym ) {
    symkind temp_sym = sym;
    int k=0;
    getsym();
    //如果+后是整数，则语法是正确的
    if (per_sym==intsym && sym==intnumbersym) {
     if (temp_sym==addsym) value = intnum;
     else {
      value = 0-intnum;
      while (intnumstr[k++]!='\0');
      intnumstr[k+1] = '\0';
      while (k!=0) {
       intnumstr[k] = intnumstr[k-1];
       k--;
      }
      intnumstr[0] = '-';
     }
     inserttable(name,kind,value,address,paranum,index);
     emit("const","int",intnumstr,name);
    }
    else if (per_sym==floatsym && sym==realnumbersym) {
     if (temp_sym==addsym) value = realnum;
     else {
      value = 0-realnum;
      while (intnumstr[k++]!='\0');
      intnumstr[k+1] = '\0';
      while (k!=0) intnumstr[k] = intnumstr[--k];
      intnumstr[k] = '-';
     }
     inserttable(name,kind,value,address,paranum,index);
     emit("const","float",intnumstr,name);
    }
    else {
     error(6);    //'='右边不合法
        while (sym!=commasym && sym!=semsym) {
         if (sym==EOFsym) exit(0);
         getsym();
     }
        return;
    }
   }
   //如果等号后出现的第一个单词是整数|实数|字符,则语法也是正确的
   else if ( (per_sym==intsym && sym==intnumbersym) || (per_sym==floatsym && sym==realnumbersym) || (per_sym==charsym && sym==ctrsym) ) {
    if (sym==intnumbersym) value = intnum;
    else if (sym==realnumbersym) value = realnum;
    else if (sym==ctrsym) value = ctr;
    inserttable(name,kind,value,address,paranum,index);
    if (kind==21) emit("const","int",intnumstr,name);
    else if (kind==22) emit("const","float",intnumstr,name);
    else if (kind==23) emit("const","char",intnumstr,name);
   }

   else {
    error(6);
    while (sym!=commasym && sym!=semsym) {
     if (sym==EOFsym) exit(0);
     getsym();
    }
    return;
   }
  }
  //如果标识符后不是等号，则表示未初始化常量
  else {
   error(7);
   while (sym!=commasym && sym!=semsym) {
    if (sym==EOFsym) exit(0);
    getsym();
   }
   return;
  }
 }
 //如果没有＜标识符＞，则常量定义的不对
 else
 {
  error(1);
  while(sym!=commasym && sym!=semsym) {
   if(sym==EOFsym) exit(0);
   getsym();
  }
  return;
 }
 getsym();
 return ;
}

void defhead()                 
{
 if (sym==intsym || sym==floatsym || sym==charsym) {
  if (sym==intsym) kind = 11;
  else if (sym==floatsym) kind = 12;
  else if (sym==charsym) kind = 13;
  getsym();  //读取变量标识符
  
  //如果是不合法的字符
  if (sym!=identitysym) {
   error(1);
   while (sym!=commasym && sym!=lparenthesessym && sym!=rparenthesessym) {
    if(sym==EOFsym) exit(0);
    getsym();
   }
      return ;
  }
  else {
   strcpy(name,id);//拷贝函数的名字
   exchange(name);//大小写转换
  }
 }
 //关键字错误
 else
 {
  error(2);            
  while (sym!=commasym && sym!=lparenthesessym && sym!=rparenthesessym) {
   if(sym==EOFsym) exit(0);
   getsym();
  }
  return ;
 }
 getsym();
 return;
}

//＜常量说明部分＞      ::=    const＜常量定义＞;{ const＜常量定义＞;}
void conststate()             
{
 symkind per_sym;
 if (sym==constsym) {
  getsym();
  if (sym==intsym || sym==floatsym || sym==charsym) {
   if (sym==intsym) kind = 21;
   else if (sym==floatsym) kind = 22;
   else if (sym==charsym) kind = 23;
   address = 0;      //对常量而言，address是没有意义的
   paranum = 0;
   per_sym = sym;
   getsym();
   
   //调用常量定义子程序
   constdef(per_sym);
   //如果是逗号，则之前的常量定义还没有结束
   while (sym==commasym) {
    getsym();
    constdef(per_sym);
   }
   //缺少';'
   if (sym!=semsym) {
    error(5);          
    while (sym!=constsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym && sym!=ifsym && sym!=whilesym
        && sym!=forsym && sym!=identitysym && sym!=returnsym && sym!=scanfsym && sym!=printfsym){
     if(sym==EOFsym) exit(0);
     getsym();
    }
    return;
   }
   sym_temp = sym;
  }
  //如果没有类型声明，则常量定义的不对
  else {
   error(2);
   while (sym!=constsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym && sym!=ifsym
       && sym!=whilesym && sym!=forsym && sym!=returnsym && sym!=scanfsym && sym!=printfsym){
    if(sym==EOFsym) exit(0);
    getsym();
   }
   return;
  }
 }
 
 //没有const关键字
 else{
  error(2);
        while (sym!=constsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym && sym!=ifsym
      && sym!=whilesym && sym!=forsym && sym!=returnsym && sym!=scanfsym && sym!=printfsym){
   if(sym==EOFsym) exit(0);
   getsym();
  }
  return ;
 }
 getsym();
 return;
}
void parametertable()
{
 int i=0;     //记录参数个数
 symkind temp;
 if( sym==intsym || sym==floatsym || sym==charsym ) {
  temp = sym;
  do{
   if (sym==commasym) getsym();
   defhead();    //函数参数和变量定义时完全相同的，只是最后不是以分号结尾
   kind = 4;     //4表示为函数参数
      value = 0;
      address = i;  //地址为i，即参数的位置，地址全部为相对地址？
      paranum = 0;
      inserttable(name,kind,value,address,paranum,index);  //将行数参数插入符号表
   if (temp==intsym) emit("para","int","  ",name);
   else if (temp==floatsym) emit("para","float","  ",name);
   else if (temp==charsym) emit("para","char","  ",name);
      i++;//参数个数加一
  }while(sym==commasym);//如果是逗号，则还有其他的参数
 }
 paranum = i;//当前的参数个数
 insertproc(paranum);//插入函数的参数个数
 return;
}
 
//＜复合语句＞   ::= ［＜常量说明部分＞］［＜变量说明部分＞］＜语句序列＞
void complexsentence()
{
 if (sym==constsym) {
  //调用常量说明子程序
  conststate();
  //如果是分号,则是正常的
  while (sym_temp==semsym) {
   if (sym==constsym)
    conststate();
   else break;
  }
 }
 //变量定义和有返回值的函数是相同的声明头部。因此预读3个单词用于判断
 while (sym==intsym || sym==floatsym || sym==charsym) {
  symkind temp = sym;
  ch_temp = ch;
  codec_temp = codec;
  if (sym==intsym) kind = 11;
  else if (sym==floatsym) kind = 12;
  else if (sym==charsym) kind = 13;
  getsym();
  strcpy(name,id);//拷贝名字
  exchange(name);//大小写转换
  //如果标识符合法
  if (sym!=identitysym) {
   error(1);   
   while (sym!=ifsym && sym!=whilesym && sym!=forsym && sym!=returnsym && sym!=scanfsym
          && sym!=printfsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym ) {
    if (sym==EOFsym) exit(0);
    getsym();
   }
   return;
  }
  getsym();
  //如果是逗号或者分号，则是变量定义
  if (sym==commasym) {
   sym = temp;
   codec = codec_temp;
   ch = ch_temp;
   varstate();
   continue;
  }
  else if (sym==semsym) {
   value = 0;
   address = 0;
   paranum = 0;
   inserttable(name,kind,value,address,paranum,index);
   if (kind==11) emit("int","  ","  ",name);
   else if (kind==12) emit("float","  ","  ",name);
   else if (kind==13) emit("char","  ","  ",name);
   getsym();
   continue;
  }
  else {
   error(5);   
   while (sym!=ifsym && sym!=whilesym && sym!=forsym && sym!=returnsym && sym!=scanfsym
          && sym!=printfsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym ) {
    if (sym==EOFsym) exit(0);
    getsym();
   }
   return;
  }
 }
 //语句列
 sentencelist();
 return ;
}
//＜变量定义＞  ::= ＜类型标识符＞＜标识符＞{,＜标识符＞}     ::=  ＜声明头部＞{，＜标识符＞}
void vardef()
{
 defhead();
 value = 0;
 address = 0;
 paranum = 0;
 inserttable(name,kind,value,address,paranum,index);
 if (kind==11) emit("int","  ","  ",name);
 else if (kind==12) emit("float","  ","  ",name);
 else if (kind==13) emit("char","  ","  ",name);
 //如果后面是逗号，那么变量定义未结束
 while (sym==commasym) {
  getsym();
  //出现不合法的标志符
  if (sym!=identitysym) {
   error(1);   
   while (sym!=ifsym && sym!=whilesym && sym!=forsym && sym!=returnsym && sym!=scanfsym
          && sym!=printfsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym ) {
    if (sym==EOFsym) exit(0);
    getsym();
   }
   return ;
  }
  strcpy(name,id);
  exchange(name);
  inserttable(name,kind,value,address,paranum,index);
  if (kind==11) emit("int","  ","  ",name);
  else if (kind==12) emit("float","  ","  ",name);
  else if (kind==13) emit("char","  ","  ",name);
  getsym();
 }
 return;
}
void varstate()
{
 vardef();
 //缺少';'
 if (sym!=semsym) {
  error(5);   
  while (sym!=ifsym && sym!=whilesym && sym!=forsym && sym!=returnsym && sym!=scanfsym
      && sym!=printfsym && sym!=intsym && sym!=floatsym && sym!=charsym && sym!=voidsym ) {
   if (sym==EOFsym) exit(0);
   getsym();
  }
  return;
 }
 getsym();
 return;
}