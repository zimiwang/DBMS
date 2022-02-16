
#include "pch.h"
#include "CppUnitTest.h"
/*
#include "../DBMSProject/bplustree.h"
#include "../DBMSProject/row.h"
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;



namespace Tests
{
	
	TEST_CLASS(Tests)
	{
	public:
		Row* rw;

		
		Row row1{ 1,1,1 };
		Row row2{ 1,1,1 };
		Row row3{ 1,1,1 };
		Row row4{ 1,1,1 };
		Row row5{ 1,1,1 };
		Row row6{ 1,1,1 };
		Row row7{ 1,1,1 };
		Row row8{ 1,1,1 };
		Row row9{ 1,1,1 };
		Row row10{ 1,1,1 };
		Row row11{ 1,1,1 };


		Column<string> column1;
		Column<string> column2;
		Column<string> column3;
		Column<string> column4;
		Column<string> column5;
		Column<string> column25;
		Column<string> column26;
		Column<string> column27;
		Column<string> column28;
		Column<string> column29;
		Column<string> column30;


		Column<char> column6;
		Column<char> column7;
		Column<char> column8;
		Column<char> column9;
		Column<char> column10;
		Column<char> column11;
		Column<char> column12;
		Column<char> column13;
		Column<char> column31;
		Column<char> column32;
		Column<char> column33;


		Column<int> column14;
		Column<int> column15;
		Column<int> column16;
		Column<int> column17;
		Column<int> column18;
		Column<int> column19;
		Column<int> column20;
		Column<int> column21;
		Column<int> column22;
		Column<int> column23;
		Column<int> column24;

		string value1 = "Rick";
		string value2 = "random";
		string value3 = "morty";
		string value4 = "alexander";
		string value5 = "anderson";
		string value25 = "man";
		string value26 = "women";
		string value30 = "big";
		string value31 = "bang";
		string value32 = "theory";
		string value33 = "time";

		char value6 = 'c';
		char value7 = 'd';
		char value8 = 'g';
		char value9 = 'f';
		char value10 = 'a';
		char value11 = 'p';
		char value12 = 'l';
		char value13 = 's';
		char value27 = 'j';
		char value28 = 'y';
		char value29 = 'o';

		int value14 = 3;
		int value15 = 23;
		int value16 = 32;
		int value17 = 92;
		int value18 = 29;
		int value19 = 31;
		int value20 = 13;
		int value21 = 82;
		int value22 = 28;
		int value23 = 2;
		int value24 = 1;



		TEST_METHOD_INITIALIZE(Populate) {


			// //string columns
			column1.AddValue(&value1);
			column2.AddValue(&value2);
			column3.AddValue(&value3);
			column4.AddValue(&value4);
			column5.AddValue(&value5);
			column25.AddValue(&value25);
			column26.AddValue(&value26);
			column27.AddValue(&value30);
			column28.AddValue(&value31);
			column29.AddValue(&value32);
			column30.AddValue(&value33);

			// character columns
			column6.AddValue(&value6);
			column7.AddValue(&value7);
			column8.AddValue(&value8);
			column9.AddValue(&value9);
			column10.AddValue(&value10);
			column11.AddValue(&value11);
			column12.AddValue(&value12);
			column13.AddValue(&value13);
			column31.AddValue(&value27);
			column32.AddValue(&value28);
			column33.AddValue(&value29);

			// int columns
			column14.AddValue(&value14);
			column15.AddValue(&value15);
			column16.AddValue(&value16);
			column17.AddValue(&value17);
			column18.AddValue(&value18);
			column19.AddValue(&value19);
			column20.AddValue(&value20);
			column21.AddValue(&value21);
			column22.AddValue(&value22);
			column23.AddValue(&value23);
			column24.AddValue(&value24);





			row1.strColumn[0] = column1;
			row1.charColumn[0] = column6;
			row1.intColumn[0] = column14;

			row2.strColumn[0] = column2;
			row2.charColumn[0] = column7;
			row2.intColumn[0] = column15;

			row3.strColumn[0] = column3;
			row3.charColumn[0] = column8;
			row3.intColumn[0] = column16;

			row4.strColumn[0] = column4;
			row4.charColumn[0] = column9;
			row4.intColumn[0] = column17;

			row5.strColumn[0] = column5;
			row5.charColumn[0] = column10;
			row5.intColumn[0] = column18;

			row6.strColumn[0] = column25;
			row6.charColumn[0] = column11;
			row6.intColumn[0] = column19;

			row7.strColumn[0] = column26;
			row7.charColumn[0] = column12;
			row7.intColumn[0] = column20;

			row8.strColumn[0] = column27;
			row8.charColumn[0] = column13;
			row8.intColumn[0] = column21;

			row9.strColumn[0] = column28;
			row9.charColumn[0] = column31;
			row9.intColumn[0] = column22;

			row10.strColumn[0] = column29;
			row10.charColumn[0] = column32;
			row10.intColumn[0] = column23;

			row11.strColumn[0] = column30;
			row11.charColumn[0] = column33;
			row11.intColumn[0] = column24;
		}         



		/// <summary>
		/// Search 1
		/// </summary>
		TEST_METHOD(TestMethod1)
		{
			BPTree node;				        

            node.insert(5,  &row1);
            node.insert(15, &row2);
            node.insert(25, &row3);
            node.insert(35, &row4);
            node.insert(45, &row5);
            node.insert(55, &row6);
            node.insert(40, &row7);
            node.insert(30, &row8);
            node.insert(20, &row9);
            
            
			Row* rw = node.search(30);
            string expected = "big";
            
			Assert::AreEqual(expected, *rw->strColumn[0].GetValue());

		}
		/// <summary>
		/// Search 2
		/// </summary>
		TEST_METHOD(TestMethod2)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);

			Row* rw = node.search(35);
			string expected = "big";

			Assert::AreNotEqual(expected, *rw->strColumn[0].GetValue());
		}

		/// <summary>
		/// Search 1
		/// </summary>
		TEST_METHOD(TestMethod3)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);

			Row* rw = node.search(5);
			char expected = 'c';

			Assert::AreEqual(expected, *rw->charColumn[0].GetValue());
		}

		/// <summary>
		/// Delete Method
		/// </summary>
		TEST_METHOD(TestMethod4)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);
			node.remove(40);

			Row* rw = node.search(40);

			Assert::IsNull(rw);
		}


		/// <summary>
		/// Delete Method
		/// </summary>
		TEST_METHOD(TestMethod5)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);
			node.remove(5);

			Row* rw = node.search(5);

			Assert::IsNull(rw);
		}


		/// <summary>
		/// Delete Method
		/// </summary>
		TEST_METHOD(TestMethod6)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);
			node.remove(40);

			Row* rw = node.search(40);

			Assert::IsNull(rw);
		}

		/// <summary>
		/// Search 4
		/// </summary>
		TEST_METHOD(TestMethod7)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);


			Row* rw = node.search(3);

			Assert::IsNull(rw);
		}


		/// <summary>
		/// Test Search Multiple
		/// </summary>
		TEST_METHOD(TestMethod8)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);


			list<Row*> rw = node.searchMultiple(5, 35);

			Assert::AreEqual(*row1.charColumn[0].GetValue(), *rw.front()->charColumn[0].GetValue());
			Assert::AreEqual(*row1.strColumn[0].GetValue(), *rw.front()->strColumn[0].GetValue());
			Assert::AreEqual(*row1.intColumn[0].GetValue(), *rw.front()->intColumn[0].GetValue());
		}


		/// <summary>
		/// Test Search Multiple
		/// </summary>
		TEST_METHOD(TestMethod9)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			node.insert(45, &row5);
			node.insert(55, &row6);
			node.insert(40, &row7);
			node.insert(30, &row8);
			node.insert(20, &row9);

			list<Row*>::iterator row;
			list<Row*> rw = node.searchMultiple(5, 35);
			Row compare1;
			Row compare2;

			Row rows[6];

			rows[0] = row1;
			rows[1] = row2;
			rows[2] = row9;
			rows[3] = row3;
			rows[4] = row8;
			rows[5] = row4;

			int i = 0;
			for (row = rw.begin(); row != rw.end(); ++row) {			
				compare1 = rows[i];
				compare2 = **row;				

				Assert::AreEqual(*compare1.charColumn[0].GetValue(), *compare2.charColumn[0].GetValue());
				Assert::AreEqual(*compare1.intColumn[0].GetValue(), *compare2.intColumn[0].GetValue());
				Assert::AreEqual(*compare1.strColumn[0].GetValue(), *compare2.strColumn[0].GetValue());
				i++;
			}
		}


		/// <summary>
		/// Test Search Multiple
		/// </summary>
		TEST_METHOD(TestMethod10)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);		

			list<Row*> rw = node.searchMultiple(5, 25);

			Assert::AreEqual(*row1.charColumn[0].GetValue(), *rw.front()->charColumn[0].GetValue());
			Assert::AreEqual(*row1.strColumn[0].GetValue(), *rw.front()->strColumn[0].GetValue());
			Assert::AreEqual(*row1.intColumn[0].GetValue(), *rw.front()->intColumn[0].GetValue());
		}

		/// <summary>
		/// Test Search Multiple
		/// </summary>
		TEST_METHOD(TestMethod11)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			
			list<Row*>::iterator row;
			list<Row*> rw = node.searchMultiple(5, 25);
			Row compare1;
			Row compare2;
			Row rows[3];

			rows[0] = row1;
			rows[1] = row2;
			rows[2] = row3;
			

			int i = 0;
			for (row = rw.begin(); row != rw.end(); ++row) {
				compare1 = rows[i];
				compare2 = **row;
				Assert::AreEqual(*compare1.charColumn[0].GetValue(), *compare2.charColumn[0].GetValue());
				Assert::AreEqual(*compare1.intColumn[0].GetValue(), *compare2.intColumn[0].GetValue());
				Assert::AreEqual(*compare1.strColumn[0].GetValue(), *compare2.strColumn[0].GetValue());
				i++;
			}
			Assert::AreEqual(3, i);
		}



		/// <summary>
		/// Test Search Multiple
		/// </summary>
		TEST_METHOD(TestMethod12)
		{
			BPTree node;

			node.insert(5, &row1);
			node.insert(15, &row2);
			node.insert(25, &row3);
			node.insert(35, &row4);
			
			list<Row*>::iterator row;
			// Key 45 is not there
			list<Row*> rw = node.searchMultiple(5, 45);

			Row compare1;
			Row compare2;
			Row rows[4];

			rows[0] = row1;
			rows[1] = row2;
			rows[2] = row3;
			rows[3] = row4;

			int i = 0;
			// should only have the four rows and nothing more
			for (row = rw.begin(); row != rw.end(); ++row) {
				compare1 = rows[i];
				compare2 = **row;
				Assert::AreEqual(*compare1.charColumn[0].GetValue(), *compare2.charColumn[0].GetValue());
				Assert::AreEqual(*compare1.intColumn[0].GetValue(), *compare2.intColumn[0].GetValue());
				Assert::AreEqual(*compare1.strColumn[0].GetValue(), *compare2.strColumn[0].GetValue());
				i++;
			}

			Assert::AreEqual(4, i);
		}

	};

}
*/
