/*
GroupMember1:- Harshil Hasmukhbhai Chakrani (129543211)
GroupMember2:- Riya Utpalkumar Mehta (129489217)
GroupMember3:- Shubham (128343217)
*/

#define _CRT_SECURE_NO_WARNINGS  
#include <iostream>   // standard library for input/output
#include <occi.h>     // library for oracle connection
#include <cstring>    // library for string functions

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee        // declared structure for the Employee
{
	int employeeNumber;
	char lastName[50];
	char firstName[50];
	char email[100];
	char phone[50];
	char extension[10];
	char reportsTo[100];
	char jobTitle[50];
	char city[50];
};

int menu(void);   // functions prototypes
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection* conn, struct Employee emp);
void displayAllEmployees(Connection* conn);
void employeeInfo(struct Employee* emp);
void insertEmployee(Connection* conn, struct Employee emp);
void updateEmployee(Connection* conn, int employeeNumber);
void deleteEmployee(Connection* conn, int employeeNumber);


int menu(void)    // declaration for menu function
{
	int num;        // declared num
	bool flag = 0;
	cout				// statment for printing menu
		<< "1) Find Employee" << endl
		<< "2) Employees Report" << endl
		<< "3) Add Employee" << endl
		<< "4) Update Employee" << endl
		<< "5) Remove Employee" << endl
		<< "0) Exit" << endl;
	do
	{
		cout << "> ";
		cin >> num;     // statment for getting input for menu

		if (cin.fail())     // if user didn't input anything
		{
			cout << "Invalid Choice!" << endl;    // error message
			cin.clear();
			flag = 0;
		}
		else
		{
			if (num >= 0 && num < 6)
			{
				flag = 1;
			}
			else
			{
				cout << "Invalid Choice!" << endl;
				cin.clear();
				flag = 0;
			}
		}
	} while (!flag);
	return num;     // return the integer
}


int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp)    // declaration of 2nd function
{
	bool status = 0;
	Statement* stmt = conn->createStatement();     // for creating statment in oracle database
	// execute the query and store it in a rs(result)
	ResultSet* rs = stmt->executeQuery("SELECT e.employeeNumber, e.lastName, e.firstName, e.email, o.phone, e.extension, es.firstName || ' ' || es.lastName AS reportsTo, e.jobTitle, o.city FROM employees e LEFT JOIN employees es ON e.reportsTo = es.employeeNumber LEFT JOIN offices o ON e.officeCode = o.officeCode WHERE e.employeeNumber= " + to_string(employeeNumber));

	if (!rs->next())     // if query execution is failed 
	{
		return 0;        // then it returns 0
	}
	else
	{
		emp->employeeNumber = employeeNumber;      // store user inputed employeenumber to the emp structure
		string Tlastname = rs->getString(2);       // get the lastname from the query result and store it in a string as Tlastname
		string Tfirstname = rs->getString(3);
		string Temail = rs->getString(4);
		string Tphone = rs->getString(5);
		string Textension = rs->getString(6);
		string TreportsTo = rs->getString(7);
		string TjobTitle = rs->getString(8);
		string Tcity = rs->getString(9);

		strcpy(emp->lastName, Tlastname.c_str());      // it converts String Tlastname to the char and store it in lastname in emp struct
		strcpy(emp->firstName, Tfirstname.c_str());
		strcpy(emp->email, Temail.c_str());
		strcpy(emp->phone, Tphone.c_str());
		strcpy(emp->extension, Textension.c_str());
		strcpy(emp->reportsTo, TreportsTo.c_str());
		strcpy(emp->jobTitle, TjobTitle.c_str());
		strcpy(emp->city, Tcity.c_str());
		return 1;                                   // it returns 1
	}
}

// for displaying an employee information
void displayEmployee(Connection* conn, struct Employee emp)  // declaration for 3rd function
{
	cout << endl;
	cout << "******  Employee Information  ******" << endl;
	cout << "employeeNumber = " << emp.employeeNumber << endl;     // display employeenumber from emp structure
	cout << "lastName = " << emp.lastName << endl;                 // display lastname from emp structure
	cout << "firstName = " << emp.firstName << endl;
	cout << "email = " << emp.email << endl;
	cout << "phone = " << emp.phone << endl;
	cout << "extension = " << emp.extension << endl;
	cout << "reportsTo = " << emp.reportsTo << endl;
	cout << "jobTitle = " << emp.jobTitle << endl;
	cout << "city = " << emp.city << endl;
};

// code for displaying all information
void displayAllEmployees(Connection* conn)       // declaration for 4th function
{
	Statement* stmt = conn->createStatement();    // for creatig statment in oracle database
	// execute the query on oracle database
	ResultSet* rs = stmt->executeQuery("SELECT e.employeenumber, e.firstname ||' '|| e.lastname, e.email, phone, e.extension, es.firstname ||' '|| es.lastname FROM employees e FULL OUTER JOIN employees es ON e.reportsto = es.employeenumber JOIN offices o ON e.officecode = o.officecode ORDER BY e.employeenumber");

	if (!rs->next())     // id there is no data store in result
	{
		cout << "There is no employees� information to be displayed.";      // displaying error message
	}
	else
	{
		cout.width(6);
		cout.setf(ios::left);
		cout << "E";
		cout.width(19);
		cout.setf(ios::left);
		cout << "Employee Name";
		cout.width(36);
		cout.setf(ios::left);
		cout << " Email ";
		cout.width(20);
		cout.setf(ios::left);
		cout << " Phone";
		cout.width(7);
		cout.setf(ios::left);
		cout << " Ext ";
		cout.width(10);
		cout.setf(ios::left);
		cout << "Manager" << endl;
		for (int i = 0; i < 103; i++) {
			cout << "-";
		}
		cout << endl;
		do            // do/while loop for displaying all employees data
		{
			cout.width(6);
			cout.setf(ios::left);
			cout << rs->getInt(1);
			cout.width(19);
			cout.setf(ios::left);
			cout << rs->getString(2);
			cout.width(36);
			cout.setf(ios::left);
			cout << rs->getString(3);
			cout.width(20);
			cout.setf(ios::left);
			cout << rs->getString(4);
			cout.width(7);
			cout.setf(ios::left);
			cout << rs->getString(5);
			cout.width(10);
			cout.setf(ios::left);
			cout << rs->getString(6) << endl;
		} while (rs->next());            // this loop will running till the last data stored in the result
	}
};

void employeeInfo(struct Employee* emp)         // this function will get the input from the user for 
{                                               // new employee and save it as a struct
	cout << "*** Add Employee ***" << endl;
	cout << "Employee Number: ";
	cin >> emp->employeeNumber;
	cout << "Last Name: ";
	cin >> emp->lastName;
	cout << "First Name: ";
	cin >> emp->firstName;
	cout << "Email: ";
	cin >> emp->email;
	cout << "extension: ";
	cin >> emp->extension;
	cout << "Job Title: ";
	cin >> emp->jobTitle;
	cout << "City: ";
	cin >> emp->city;
};

void insertEmployee(Connection *conn, struct Employee emp)  // this function for inserting employee info
{

	char fsofficeCode = 1;
	char fsreportsTo = 1002;

	string officeCode(1, fsofficeCode);
	string reportsTo(2, fsreportsTo);

	if (findEmployee(conn, emp.employeeNumber, &emp) == 1)      // this will call findemployee function and check 
	{                                                           // that this employee exits or not
		cout << "Employee is already exits";
	}
	else                                                       // if the employee does not exits than this will insert it
	{

		Statement* stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO employees VALUES(:1,:2,:3,:4,:5,:6,:7, :8)");

		stmt->setInt(1, emp.employeeNumber);
		stmt->setString(2, emp.lastName);
		stmt->setString(3, emp.firstName);
		stmt->setString(4, emp.extension);
		stmt->setString(5, emp.email);
		stmt->setString(6, officeCode);
		stmt->setString(7, reportsTo);
		stmt->setString(8, emp.jobTitle);

		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);
		cout << "\nThe new employee is added successfully.";
	}
};

void updateEmployee(Connection* conn, int employeeNumber)       // this function for update employee information
{
	struct Employee fCopy;
	string extension;


	if (findEmployee(conn, employeeNumber, &fCopy) == 1)  // this function will search for employee
	{
		cout << "New Extension: ";                        // if employee exist than it will ask for new extension
		cin >> extension;

		Statement* stmt = conn->createStatement();	
		stmt->setSQL("UPDATE employees SET extension=:1 WHERE employeeNumber=:2");

		stmt->setString(1, extension);
		stmt->setInt(2, employeeNumber);

		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);

		cout << "The extension column of the row with the " << employeeNumber << " will be updated with the new value " << extension << ".";
	}
	else              // if employee does not exist than it will return following statment
	{
		cout << "The employee " << employeeNumber << " does not exits.";
	}
};

void deleteEmployee(Connection *conn, int employeeNumber)       // this function will delete the employee information
{
	struct Employee fCopy;

	if (findEmployee(conn, employeeNumber, &fCopy) == 1)        // this will search for employee information
	{
		Statement* stmt = conn->createStatement();
		stmt->setSQL("DELETE FROM employees WHERE employeeNumber=:1"); // this statment delete the employee info

		stmt->setInt(1, employeeNumber);
		stmt->executeUpdate();
		conn->commit();
		conn->terminateStatement(stmt);

		cout << employeeNumber <<  " is deleted.";
	}
	else                  // this function will return following statment if the employee info doesn't found
	{
		cout << "The employee " << employeeNumber << " does not exist.";
	}
};


int main(void)           // entry point for application
{
	Environment* env = nullptr;     // declared pointer
	Connection* conn = nullptr;

	string str;
	string usr = "dbs211_221za06";      // username for the oracle database
	string pass = "14640566";           // password for oracle database
	string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";

	try
	{
		env = Environment::createEnvironment(Environment::DEFAULT);      // create the environment for connection
		conn = env->createConnection(usr, pass, srv);                      // make the connection ussing username and password

		cout << "********************* HR Menu *********************" << endl;
		int num;
		Employee e1;
		do                        // do/while loop for implementing the function
		{
			num = menu();
			switch (num)          // used switch/ case for different options
			{
			case 1:               // case 1 for displaying information about an employee
			{
				int Number;
				bool flag = 0;

				do {
					cout << "Enter Employee Number: ";
					cin >> Number;      // statment for input the employee number
					if (cin.fail())     // if user didn't input anything
					{
						cout << "Invalid Input";    // it will saw the error message
						cin.clear();
						flag = 0;
					}
					else {             // if user input valid input 
						if (findEmployee(conn, Number, &e1) == 1)  //call the findEmployee function -- if this function returns 1
						{
							displayEmployee(conn, e1);    // display the information for an employee
						}
						else    // if findemployee function returns 0
						{
							cout << "Employee " << Number << " does not exist!" << endl;   // it will saw a error message
						}
						flag = 1;
					}
				} while (!flag);    // this will run untill the condition is not true
				break;
			}
			case 2:                // case 2 for displayion information for all employees
				displayAllEmployees(conn);    // call the displayAllEmployees function
				break;

			case 3:   // case 3 for inserting employees information
			{
				employeeInfo(&e1);
				insertEmployee(conn, e1);
				break;
			}
			case 4:   // case 4 for updating employees information
			{
				cout << "Employee Number: ";
				cin >> num;
				updateEmployee(conn, num);
				break;
			}
			case 5:   // case 5 for delete employee information
			{
				cout << "Employee Number: ";
				cin >> num;
				deleteEmployee(conn, num);
			}
			}
		} while (num < 0 && num > 5);


		env->terminateConnection(conn);         // this statment terminate the connection
		Environment::terminateEnvironment(env);   // this statment terminate the environment
	}
	catch (SQLException& sqlExcp)          // this class for displaying error in sql statment
	{
		cout << "Error Code is  " << sqlExcp.getErrorCode() << " And Message is " << sqlExcp.getMessage();
	}
	return 0;
}