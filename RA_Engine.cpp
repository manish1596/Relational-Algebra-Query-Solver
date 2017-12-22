#include <bits/stdc++.h>
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#define MAX 100
using namespace std;

//attribute class is the class of the attributes associated with the different tables of the database 
class attribute{
public:
  string name;
  string datatype;
} ;

//The table class stores the attributes associated with the table object int he vector attr and the data of the table is 
//stored in the 2D string vector tabledata. All data, including numeric and date values are stored as strings in this 2D vector
//However since the attr vector stores the datatype associated with the individual attributes, we can always find the specific 
//datatype associated with the tabledata values from there.  
class table{
public:
  string name;
  vector<attribute> attr;
  vector< vector<string> > tabledata;
} tset[MAX];

//The tset array is an array of table objects. This array is the workspace in which all tables are loaded and the workspace in
//which the program stores new tables as and when they are created.

int cn=-1;
//cn is a global variable which stores the number of tables of the workspace that are currently filled. To allocate a new table
//a function must increment cn.Since initially all tables are empty, cn has been initialised with -1.

//Function load_table loads a table in the workspace. Arguments of this function are the table of the workspace in which
//the new table has to be loaded and the filename of the file from which the new table has to be loaded.
//Also the text file to be loaded in the workspace must adhere to a specific layout for it to be loaded successfully:

/*
Specifications for the text files:
All the attribute,datatype pairs must be listed out in the first line. Every such pair must be followed by
the character '|' to separate it from a different pair
In the successive lines, the tuples to be stored in the table must be listed out in the rows of the file. Again every 
attribute value must be separated from every other attribute value by the character '|'. There may be spaces between the 
attribute names and their values but there must not be any spaces other than these.
Datatypes supported: 
1. int: numeric data
2. varchar: any string
3. date: date in the format YYYY-MM-DD
*/


void load_table(table &T, string filename){
    T.name=filename;
    int i=-1;
    string s, s2;
    ifstream tfile(filename.c_str());
    getline(tfile, s);
    istringstream iss(s);
    while(getline(iss, s2, '|')) {
      istringstream iss1(s2);
      attribute Atemp;
      getline(iss1, Atemp.name, ',');
      getline(iss1, Atemp.datatype);
      T.attr.push_back(Atemp);
    }
    while(getline(tfile, s)){
      vector<string> v;
      i++;
      T.tabledata.push_back(v);
      istringstream iss(s);
      while(getline(iss, s2, '|')){
      T.tabledata[i].push_back(s2);
      }
    }
  }



//Function create_table creates a new table. the argument string of this function is the specification of the new table.
//The string st must be of the following form : (table-name) (attributes-datatypes separated by commas) 
table create_table(string st){
  string s;
  istringstream iss(st);
  table T;
  getline(iss, T.name, ' ');
  while(getline(iss, s, ',')){
    istringstream iss1(s);
    attribute A;
    getline(iss1, A.name, '-');
    getline(iss1, A.datatype);
    T.attr.push_back(A);
  }
  return T;
}

//This function saves a table of the workspace in a text file. The text file so created will adhere to the specifications for the 
//text files as described above.  
void save_table(table T){
  ofstream out( (T.name).c_str() );
  int i, j, n=T.attr.size();;
  
  for(i=0; i<n; i++){
    out << T.attr[i].name;
    out << ",";
    out << T.attr[i].datatype;
    out << "|";
  }
  out << "\n";
  for(i=0; i<T.tabledata.size(); i++){
    for(j=0; j<n; j++){
      out << T.tabledata[i][j];
      out << "|";
    }
    out << "\n";
  }
}


//checks if the tuple represented by data is already present in the table T
bool check_if_dup(table T, vector<string> data){           
  int i, j;
  bool b;
  for(i=0; i<T.tabledata.size(); i++){
    b=true;
    for(j=0; j<T.tabledata[i].size(); j++){
      if(data[j]!=T.tabledata[i][j]){
        b=false;
        break;
      }
    }
    if(b==true)
      return true;
  }
  return false;
}

//The following function checks if the string word contains only numeric data and therefore can be accepted as a valid int entry
bool valid_int(string word){
  bool isNumber = true;
  for(string::const_iterator k = word.begin(); k != word.end(); ++k)
    isNumber = isNumber && isdigit(*k);
  return isNumber;
}

//The following function checks if the string word is in the correct date format and therefore can be accepted as a valid date entry
bool valid_date(string word){
  if( word.size()==10 && word[4]=='-' && word[7]=='-' && isdigit(word[0]) && isdigit(word[1]) && isdigit(word[2]) && isdigit(word[3]) && 
    isdigit(word[5]) && isdigit(word[6]) && isdigit(word[8]) && isdigit(word[9]) )
    return true;
  else 
    return false;
}

//Function to insert values in the table T. The string st consists of comma separated attribute values.
void insert_into(table& T, string st){
  istringstream iss(st);
  string s;
  vector<string> v;
  while(getline(iss, s, ','))
    v.push_back(s);
  if(check_if_dup(T, v)){
    cerr << "ERROR : Duplicate tuple cannot be inserted.\n";
    return;
  }
  else{
    for(int a=0; a<T.attr.size(); a++){
      if(T.attr[a].datatype=="int"){
        if(!valid_int(v[a])){
          cerr << "ERROR : Invalid datatype at column " << a+1 << "\n";
          return;
        }
      }
      if(T.attr[a].datatype=="date"){
        if(!valid_date(v[a])){
          cerr << "ERROR : Invalid datatype at column " << a+1 << "\n";
          return;
        }
      }
    }
    T.tabledata.push_back(v);
  }
}

//This function returns the comparison operator used in the condition string s.
char comp_op(string s){
  for(int i=0; i<s.size(); i++){
    if(s[i]=='=' || s[i]=='<' || s[i]=='>')
      return s[i];
  }
}


//This function evaluates the condition op1 op op2 where op is the comparison operator and op1, op2 are the operands which
//have to be compared. In case of inequality comparisons, it is assumed that the operands are of int datatype.
bool eval_condition(string op1, char op, string op2){
  if(op=='='){
    return op1==op2;
  }
  else if(op=='<'){
    return atoi(op1.c_str())<atoi(op2.c_str());
    
  }
  else if(op=='>'){
    return atoi(op1.c_str())>atoi(op2.c_str());
  }
}

//This function returns true if the operator represented by character op2 has higher precedence than the operator represented
//by the character op2.
bool Precedence(char op1, char op2)
    {
        if (op2 == '(' || op2 == ')')
            return false;
        if(  (op1 == '!') && (op2=='&' || op2=='|') )
            return false;
        if(op1 == '&' && op2=='|')
            return false;
        else
            return true;
    }

//This function applies the operation a op b and returns the output.
bool applyOp(char op, bool a, bool b)
    {
        if(op=='&')
          return a && b;
        else
          return a || b;
    }


//This function evaluates the individual conditions and replaces them by their truth values (for the given tuple, v) in the condition string and returns the string so obtained
string eval_exp(map<string, int> &idx, string s, vector<string> &v){
  int i, j;
  string ret="";
  i=0;
  while(1){
    if(i>=s.length()){
      break;
    }
    while( s[i]=='&' || s[i]=='|' || s[i]=='!' || s[i]=='(' || s[i]==')' ){
      ret=ret+s[i];
      i++;
      if(i>=s.length()){
        break;
    }
    }
    if(i>=s.length()){
      break;
    }
    char temp[500];
    j=0;
    while(s[i]!='&' && s[i]!='|' && s[i]!='!' && s[i]!='(' && s[i]!=')'){
      temp[j]=s[i];
      j++; i++;
      if(i>=s.length()){
        break;
    }
    }
    temp[j]='\0';
    string strtemp(temp);
    istringstream isstemp(strtemp);
    string attrname, checkval;
    char cop=comp_op(strtemp);
    getline(isstemp, attrname, cop);
    getline(isstemp, checkval);
    if(idx.find(attrname)==idx.end()){
      return "";
    }
    if(idx.find(checkval)!=idx.end()){
      bool res=eval_condition(v[idx[attrname]], cop, v[idx[checkval]]);
      if(res)
        ret=ret+'1';
      else
        ret=ret+'0';
      continue;
    }
    bool res=eval_condition(v[idx[attrname]], cop, checkval);
    if(res)
      ret=ret+'1';
    else
      ret=ret+'0';
  }
  return ret;
}

//This function parses the conditions after their truth values have been determined by the eval_exp function. The function stores
//the indices for which the condition string s holds true in a vector and returns this vector to the calling function.
vector<int> parse_condition(string s, table &t){
  int i, j;
  map<string, int> attr_idx;
  for(i=0; i<t.attr.size(); i++){
    attr_idx[t.attr[i].name]=i;
  }
  vector<int> selected;
  for(i=0; i<t.tabledata.size(); i++){
    stack<bool> bits;
    stack<char> op;
    string stemp=eval_exp(attr_idx, s, t.tabledata[i]);
    if(stemp==""){
      selected.push_back(-1);
      return selected;
    }
    for(j=0; j<stemp.length(); j++){
      if(stemp[j]==' ')
        continue;
      if(stemp[j]=='0' || stemp[j]=='1'){
        bits.push(stemp[j]-'0');
      }
      else if(stemp[j]=='(')
        op.push(stemp[j]);
      else if(stemp[j]==')'){
        while(op.top()!='('){
          bool bit1=bits.top(); bits.pop();
          if(op.top()=='!'){
            bits.push(!bit1);
            op.pop();
            continue;
          }
          bool bit2=bits.top(); bits.pop();
          bits.push(applyOp(op.top(), bit2, bit1));
          op.pop();
        }
        op.pop();
      }
      else if(stemp[j]=='&' || stemp[j]=='|' || stemp[j]=='!'){
        while(!op.empty() && Precedence(stemp[j], op.top())){
          bool bit1=bits.top(); bits.pop();
          if(op.top()=='!'){
            bits.push(!bit1);
            op.pop();
            continue;
          }
          bool bit2=bits.top(); bits.pop();
          bits.push(applyOp(op.top(), bit2, bit1));
          op.pop();
        }
        op.push(stemp[j]);  
      }
    }
    while(!op.empty()){
      bool bit1=bits.top(); bits.pop();
        if(op.top()=='!'){
          bits.push(!bit1);
          op.pop();
          continue;
        }
        bool bit2=bits.top(); bits.pop();
        bits.push(applyOp(op.top(), bit2, bit1));
        op.pop();
    }
    if(bits.top())
      selected.push_back(i);
  }
  return selected;
}

//This is the recursive function which handles all the queries and returns the table number of the table in which it stores the
//output of the query string S. If the query simply asks to retrieve or save an existing table, the function searches the 
//workspace for the queried table and returns its number. This function also shows the output of the queries on the standard
//output. In case the query is erroneous the function returns -1.
//Since this is a recursive function, it can handle nested queries.

/*File-naming conventions for outputs of the queries:
SELECT query : The output of SELECT query is named as s_tablename, where tablename is the name of table on which the query is executed
PROJECT query : The output of PROJECT query is named as p_tablename
CARTESIAN_PRODUCT : table1_PROD_table2
UNION : table1_UNION_table2
SET_DIFFERENCE : table1_SETDIFF_table2
NATURAL_JOIN : table1_JOIN_table2
In case many tables with the same name exist in the workspace, the most recent created table of that name will be retrieved and saved.  
*/

int process_query(string S){
  istringstream iss(S);
  
  string s1, s2;
  getline(iss, s1, ' ');
  int tablematch=-1;
  for(int a=MAX-1; a>=0; a--){
      if(tset[a].name==s1){
        tablematch=a;
        break;
      }
    }
  if(tablematch!=-1)
    return tablematch;

  if(s1=="CREATE_TABLE"){
    getline(iss, s2);
    cn++;
    tset[cn]=create_table(s2);
    return cn;
  }
  else if(s1=="INSERT_INTO"){
    int temp;
    string tname;
    getline(iss, tname, ' ');
    for(int a=0; a<100; a++){
      if(tset[a].name==tname){
        temp=a;
        break;
      }
    }
    getline(iss, s2);
    insert_into(tset[temp], s2);
    return temp;
  }
  else if(s1=="LOAD_TABLE"){
    getline(iss, s2);
    cn++;
    load_table(tset[cn], s2);
    return cn;
  }
  else if(s1=="SAVE"){
    int temp=-1;
    string tname;
    getline(iss, tname);
    for(int a=MAX-1; a>=0; a--){
      if(tset[a].name==tname){
        temp=a;
        break;
      }
    }
    if(temp!=-1)
      save_table(tset[temp]);
    else
      cerr << "No such table exists";
    return temp;

  }
  else if(s1=="PROJECT"){
    getline(iss, s2, ':');
    istringstream iss2(s2);
    vector<string> projected_attr;
    string h;
    while(getline(iss2, h, ',')){
      projected_attr.push_back(h);
    }
    string query;
    getline(iss, query);
    int outputidx=process_query(query);
    if(outputidx==-1) return -1;
    cn++;
    vector<int> pos;
    for(int q=0; q<projected_attr.size(); q++){
      bool flag=false;
      for( int s=0; s<tset[outputidx].attr.size(); s++ ){
        if(tset[outputidx].attr[s].name==projected_attr[q]){
          flag=true;
          break;
        }
      }
      if(flag==false){
        cerr << "ERROR : You are trying to project a non-existent attribute\n";
        return -1;
      }
    }
    for(int w=0; w<tset[outputidx].attr.size(); w++){
      if( find(projected_attr.begin(), projected_attr.end(), tset[outputidx].attr[w].name ) != projected_attr.end() ) {
        pos.push_back(w);
        tset[cn].attr.push_back(tset[outputidx].attr[w]);
      }
    }
    for(int y=0; y<tset[outputidx].tabledata.size(); y++){
      vector<string> u;
      for(int z=0; z<pos.size(); z++){
        u.push_back(tset[outputidx].tabledata[y][pos[z]]);
      }
      tset[cn].tabledata.push_back(u);
    }
    tset[cn].name=string("p_") + tset[outputidx].name;
    return cn;
  }
  //SELECT queries having any conditions connected by &, |, ! and having brackets can be handled provided that in every individual
  //atomic condition the lhs consists of an attribute name and the rhs consists of some attribute value.
  else if(s1=="SELECT"){
    string t_names, condition, temp , temp1, temp2;
    getline(iss, condition, ':');
    getline(iss, t_names);
    istringstream iss2(t_names);
    int u=process_query(t_names);
    if(u==-1) return -1;
    vector<int> select=parse_condition(condition, tset[u]);
    if(select.size()>0){
      if(select[0]==-1){
        cerr << "ERROR : One or more non-existent attributes are present in the condition.\n";
        return -1;
      }
    }
    cn++;
    for(int c=0; c<tset[u].attr.size(); c++){
      tset[cn].attr.push_back(tset[u].attr[c]);
    }
    for(int r=0; r<select.size(); r++){
      tset[cn].tabledata.push_back(tset[u].tabledata[select[r]]);
    }
    tset[cn].name=string("s_")+tset[u].name;
    return cn;
  }
  else if(s1=="SHOW"){
    getline(iss, s2);
    int d=process_query(s2);
    if(d==-1) return -1;
    cout << "                          " << tset[d].name << "\n";
    for(int o=0; o<tset[d].attr.size(); o++){
      cout << setw(25) << tset[d].attr[o].name + "," + tset[d].attr[o].datatype ;
    }
    cout << "\n";
    for(int o=0; o<tset[d].tabledata.size(); o++){
      for (int l = 0; l < tset[d].tabledata[o].size(); ++l){
        cout << setw(25) << tset[d].tabledata[o][l];
      }
      cout << "\n";
    }
    return d;
  }
  else if(s1=="RENAME"){
    string new_def, temp;
    getline(iss, s2, '~');
    int u=process_query(s2);
    if(u==-1) return -1;
    getline(iss, new_def);
    istringstream iss2(new_def);
    cn++;
    getline(iss2, tset[cn].name, ' ');
    vector<string> vec;
    while(getline(iss2, temp, ','))
      vec.push_back(temp);
    for(int y=0; y<tset[u].attr.size(); y++){
      attribute a;
      a.name=vec[y];
      a.datatype=tset[u].attr[y].datatype;
      tset[cn].attr.push_back(a);
    }
    for(int y=0; y<tset[u].tabledata.size(); y++){
      tset[cn].tabledata.push_back(tset[u].tabledata[y]);
    }
    return cn;
  }
  else if(s1=="UNION"){
    getline(iss, s2, '+');
    int f = process_query(s2);
    if(f==-1) return -1;
    getline(iss, s2);
    int l = process_query(s2);
    if(l==-1) return -1;
    if( tset[f].attr.size() != tset[l].attr.size() ){
      cerr << "Incompatible tables";
      return -1;                                                     
    } 
    int i, j;
    for( i = 0; i < tset[f].attr.size(); i++ ){
      if( tset[f].attr[i].name != tset[l].attr[i].name || tset[f].attr[i].datatype != tset[l].attr[i].datatype ){
        cerr << "Incompatible tables";
        return -1;
      }
    }
    cn++;
    tset[cn].name=tset[f].name+"_UNION_"+tset[l].name;
    for(i=0; i<tset[f].attr.size(); i++){
      tset[cn].attr.push_back(tset[f].attr[i]) ;
    }
    for(i=0; i<tset[f].tabledata.size(); i++){
      tset[cn].tabledata.push_back(tset[f].tabledata[i]);
    }
    for(i=0; i<tset[l].tabledata.size(); i++){
      if(check_if_dup(tset[f], tset[l].tabledata[i])){
        continue;
      }
      else{
        tset[cn].tabledata.push_back(tset[l].tabledata[i]);
      }
    }
    return cn;
  }
  else if(s1=="CARTESIAN_PRODUCT"){
  	int i, j;
  	getline(iss, s2, 'X');
    int f = process_query(s2);
    if(f==-1) return -1;
    getline(iss, s2);
    int l = process_query(s2);
    if(l==-1) return -1;
    cn++;
    attribute a;
    tset[cn].name=tset[f].name+"_PROD_"+tset[l].name;
    for(i=0; i<tset[f].attr.size(); i++){
    	a.name=tset[f].name + "." + tset[f].attr[i].name;
    	a.datatype=tset[f].attr[i].datatype;
    	tset[cn].attr.push_back(a);
    }
    for(i=0; i<tset[l].attr.size(); i++){
    	a.name=tset[l].name + "." + tset[l].attr[i].name;
    	a.datatype=tset[l].attr[i].datatype;
    	tset[cn].attr.push_back(a);
    }
    for(i=0; i<tset[f].tabledata.size(); i++){
    	for(j=0; j<tset[l].tabledata.size(); j++){
    		vector<string> temp;
    		temp.insert(temp.begin(), tset[f].tabledata[i].begin(), tset[f].tabledata[i].end());
    		temp.insert(temp.end(), tset[l].tabledata[j].begin(), tset[l].tabledata[j].end() );
    		tset[cn].tabledata.push_back(temp);
    	}
    }
    return cn;
  }
  else if(s1=="SET_DIFFERENCE"){
    int i, j;
    getline(iss, s2, '-');
    int f = process_query(s2);
    if(f==-1) return -1;
    getline(iss, s2);
    int l = process_query(s2);
    if(l==-1) return -1;
    if( tset[f].attr.size() != tset[l].attr.size() ){
      cerr << "Incompatible tables";
      return -1;                                                     
    }
    for( i = 0; i < tset[f].attr.size(); i++ ){
      if( tset[f].attr[i].name != tset[l].attr[i].name || tset[f].attr[i].datatype != tset[l].attr[i].datatype ){
        cerr << "Incompatible tables";
        return -1;                                                    
      }
    }
    cn++;
    tset[cn].name=tset[f].name+"_SETDIFF_"+tset[l].name;
    for(i=0; i<tset[f].attr.size(); i++){
      tset[cn].attr.push_back(tset[f].attr[i]) ;
    }
    for(i=0; i<tset[f].tabledata.size(); i++){
      if(check_if_dup(tset[l], tset[f].tabledata[i]))
        continue;
      tset[cn].tabledata.push_back(tset[f].tabledata[i]);
    }
    return cn;
  }
  else if(s1=="NATURAL_JOIN"){
    getline(iss, s2, ';');
    int l = process_query(s2);
    if(l==-1) return -1;
    getline(iss, s2);
    int f = process_query(s2);
    if(f==-1) return -1;
    vector<int> common;
    int i, j;

    for(i=0; i<tset[f].attr.size(); i++){
      for(j=0; j<tset[l].attr.size(); j++){
        if(tset[l].attr[j].name==tset[f].attr[i].name && tset[l].attr[j].datatype==tset[f].attr[i].datatype){
          common.push_back(i);
          break;
        }
      }
    }

    if(common.size()==0){
      return process_query("CARTESIAN_PRODUCT "+tset[l].name+"X"+tset[f].name);
    }
    cn++;
    int toret=cn;
    tset[cn].name=tset[l].name+"_JOIN_"+tset[f].name;
    
    for(i=0; i<tset[l].attr.size(); i++){
      tset[cn].attr.push_back(tset[l].attr[i]);
    }
    i=0;
    j=0;
    while(i<tset[f].attr.size()){
      if(i==common[j]){
        j++;
        i++;
        continue;
      }
      tset[cn].attr.push_back(tset[f].attr[i]);
      i++;
    }
    
    int w=process_query("CARTESIAN_PRODUCT "+tset[l].name+"X"+tset[f].name);
    for(j=0; j<tset[f].tabledata.size(); j++){
      string join_condition="SELECT ";
      for(i=0; i<common.size(); i++){
        join_condition=join_condition+tset[l].name+"."+tset[f].attr[common[i]].name+"="+tset[f].tabledata[j][common[i]]+'&';
        
      }
      for(i=0; i<tset[f].attr.size(); i++){
        join_condition=join_condition+tset[f].name+"."+tset[f].attr[i].name+"="+tset[f].tabledata[j][i]+'&';
      }
      join_condition=join_condition.substr(0,join_condition.length()-1);
      join_condition=join_condition+":"+tset[w].name;
      int temp=process_query(join_condition);
      for(int h=0; h<tset[temp].tabledata.size(); h++){
        vector<string> vec;
        int g, e;
        for(g=0; g<tset[l].attr.size(); g++){
          vec.push_back(tset[temp].tabledata[h][g]);
        }
        e=0;
        while(g<tset[temp].attr.size()){
          if(g==common[e]+tset[l].attr.size()){
            e++;
            g++;
            continue;
          }
          vec.push_back(tset[temp].tabledata[h][g]);
          g++;
        }

        tset[toret].tabledata.push_back(vec);
      }
      if(cn>toret+1){
        tset[cn].name="";
        tset[cn].attr.clear();
        tset[cn].tabledata.clear();
        cn--;
      }
    }
    return toret;
  }
}



int main(){
  cout << "///////////////////////////////////////////////////////////////////////////\n";
  cout << "                          Relational Algebra Engine\n";
  cout << "                       Submitted By : Manish Kumar Singh\n";
  cout << "                                Roll No. : 14075065\n";
  cout << "                                   Welcome!!" << "\n";
  cout << "///////////////////////////////////////////////////////////////////////////\n";
  cout << "1. CREATE_TABLE (table_name) (attributes-datatypes separated by commas)\n";
  cout << "2. INSERT_INTO (table_name) (values to insert separated by commas)\n";
  cout << "3. LOAD_TABLE filename\n";
  cout << "4. SAVE tablename\n";
  cout << "5. SHOW tablename\n";
  cout << "6. PROJECT (attributes separated by commas):(tablename)\n";
  cout << "7. SELECT (condition):(table_name)         (condition must consist of AND(&), OR(|), NOT(!) operators only, there should be nospaces unless they are present in the names of the attributes themselves and >, < or = may be used to compare attribute values. Inequalities between integer attributes only shall be evaluated.)\n";
  cout << "8. RENAME table_name~new_name (att1, att2, ....)\n";
  cout << "9. UNION table1+table2\n" ;
  cout << "10. CARTESIAN_PRODUCT table1Xtable2\n";      //X is used as a special symbol here, therefore it must not be used anywhere in the description of the two tables
  cout << "11. SET_DIFFERENCE table1-table2\n";      //- is used as a special symbol here, therefore it must not be used anywhere in the description of the two tables 
  cout << "12. NATURAL_JOIN table1;table2\n";
  cout << "13. EXIT\n";
  string S;
  int x;
  while(1){
  getline(cin, S, '\n');
  if(S=="EXIT")
  	break;
  x=process_query(S);
  if(x==-1) continue;
  if(!((S[0]=='C' && S[1]=='R') || (S[0]=='I' && S[1]=='N') || (S[0]=='S' && S[1]=='A') || (S[0]=='S' && S[1]=='H')))
    process_query("SHOW "+tset[x].name);
  }
}
