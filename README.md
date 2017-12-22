# Relational-Algebra-Query-Solver
This work is an attempt to simulate the basic operations of the relational algebra. Using this engine, you can create tables, insert data into them, load tables saved in text files and use the basic operations of relational algebra to extract useful data from these tables. The files are described below:
1. RA_Engine.cpp : This is the source code file of the project.
2. RA.exe : This is the executable file of the source code.
These file contains a sample table for tseting purposes:
3.Faculty.txt
4.Student1.txt
5.Student2.txt
This file consists of some sample queries executed on the above tables and their outputs.

To compile the above cpp code run the following command in the terminal:
g++ RA_Engine.cpp -o RA
and to run the executable file so obtained run the following command in the terminal:
./RA

Specifications for the text files:
All the attribute,datatype pairs must be listed out in the first line.
Every such pair must be followed by the character '|' to separate it from a different pair. In the successive lines, the tuples to be stored in the table must be listed out in the rows of the file. Again every attribute value must be separated from every other attribute value by the character '|'. There may be spaces between the attribute names and their values but there must not be any spaces other than these.Datatypes supported:
1. int: numeric data
2. varchar: any string
3. date: date in the format YYYY-MM-DD
File-naming conventions for outputs of the queries:

SELECT query : The output of SELECT query is named as 
s_tablename, where tablename is the name of table on which the query is executed

PROJECT query : The output of PROJECT query is named as
p_tablename

CARTESIAN_PRODUCT : table1_PROD_table2

UNION : table1_UNION_table2

SET_DIFFERENCE : table1_SETDIFF_table2

NATURAL_JOIN : table1_JOIN_table2

In case many tables with the same name exist in the workspace, the most recent created table of that name will be retrieved and saved.

Assumptions:
1. Comparison across datatypes is not allowed.
2. All text files being loaded in the workspace are assumed to adhere to the specifications stated above.
