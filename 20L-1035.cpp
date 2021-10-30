#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<iomanip>

# define MaxFriend 10
# define MaxPages 10
# define MaxLikes 10
# define MaxComments 10

using namespace std;

class Page;
class User;
class FaceBook;
class Post;
class Comment;
class Date;
class Activity;
class Object;

//_______________________________________________________________________//
class Object// Parent class for Users and Pages
{
protected:
	char* ID;
public:
	Object();
	virtual ~Object();
	virtual void PrintName() {		}
	virtual void SetPosts(Post*& List) {		}
	virtual void PrintLikes() {		}
};

Object::Object()
{
	ID = NULL;
}

Object::~Object()
{
	if (ID)
		delete[] ID;
}

//___________________________________________________________________________//
class Helper// helper  class for memory allocation
{
public:
	static int StringLenght(char* str)
	{
		int length = 0;
		while (*str != '\0')// counts until null pointer is reached
		{
			length++;
			str++;
		}
		return length;
	}

	static char* GetStringFromBuffer(char* str)// Gets strings measure its length and allocate memory
	{
		int length = StringLenght(str);
		char* space = new char[++length];
		int i = 0;
		for (; str[i] != '\0'; i++)
			space[i] = str[i];
		space[i] = '\0';
		return space;
	}
};

//___________________________________________________________________________//
class Date
{
private:
	int Day;
	int Month;
	int Year;
public:
	Date();
	static Date Current_Date;// Object to store current Date
	void setDate(int d, int m, int y);//Setters for class
	void SetDate(int a) { Day = a; }
	void SetMonth(int a) { Month = a; }
	void SetYear(int a) { Year = a; }
	void Get() { cout <<"( " << Day << "/" << Month << "/" << Year<<" )"; }
	bool LatestCheck()// To check latest
	{
		return Day == Current_Date.Day - 1 || Month == Current_Date.Month - 1 || Year == Current_Date.Year - 1 || Day==Current_Date.Day;
	}
};

Date::Date()//constructor
{
	Day = 0;
	Month = 0;
	Year = 0;
}

void Date::setDate(int d, int m, int y)//setter
{
	Day = d;
	Month = m;
	Year = y;
}
Date Date::Current_Date;// current static date initialization
//___________________________________________________________________________//
class Comment
{
private:
	char* Text;
	char* ID;
	Object* Author;
public:
	Comment();
	~Comment();
	char* GetText()					{				return Text;			}// setters and getter
	void SetId(char* id)			{ ID = Helper::GetStringFromBuffer(id); }
	void SetText(char* name)		{ Text = Helper::GetStringFromBuffer(name);}
	void SetAuthor(Object*& Name)	{				Author = Name;			}
	void Print();
};

void Comment::Print()// Print writer of comment and his text
{
	Author->PrintName();
	cout << Text << endl;
}

Comment::Comment()//constructor
{
	Text = NULL;
	ID = NULL;
	Author = NULL;
}

Comment::~Comment()// destructor
{
	if (Text)
		delete[] Text;
	if (ID)
		delete[] ID;
	Author = NULL;
}


//___________________________________________________________________________//
class Post
{
protected:
	char* Text;
	char* ID;
	Comment** Comments;
	Date Posted_On;
	Object* Author;
	Object** LikedBy;
	int CommentCounter;
	int LikesCounter;
public:
	Post();
	virtual  ~Post();
	virtual void Print();
	void AddLike(Object*& );
	void AddComment(const char* CID, const char* Text, Object*& Author);// Add comments in List
	void PrintTimeline();
	void PrintAll();//Prints all who Liked Post
	virtual void LoadFromFile(ifstream& File);// Loads Posts
	void AggregateComments(Comment*& comment);// Aggregates comments with Posts
	void Search(const char* String);// Search the comments related to given string

	void SetAuthor(Object*& writer)		{ Author = writer; }// setters and getters
	void SetLikers(Object**& writer)	{ LikedBy = writer; }
	void SetLikes(int C)				{	LikesCounter = C;}
	char* GetText() { return Text;		}
	char* GetID()	{ return ID;		}
	Date GetDate()	{ return Posted_On; }
};

void Post::AddComment(const char* CID, const char* Text, Object*& Author)
{
	char* temp = const_cast<char*>(CID);
	Comments[CommentCounter] = new Comment;// Creates a new object to store new comment and sets values
	Comments[CommentCounter]->SetId(temp);

	char* temps = const_cast<char*>(Text);
	Comments[CommentCounter]->SetText(temps);
	Comments[CommentCounter]->SetAuthor(Author);
	CommentCounter++;// increment counter
}

void Post::PrintTimeline()// Print Post along with author and comments
{
	Author->PrintName();
	cout << " Shared: " << Text;
	Posted_On.Get();
	cout << endl;
	for (int i = 0; i < CommentCounter; i++)
		Comments[i]->Print();
	cout << endl;
}

void Post::Search(const char* String)
{
	for (int i = 0; i < CommentCounter; i++)
		if (strstr(Text, Comments[i]->GetText()))// if the Given string matches Print comment
		{
			cout << ID << " : " << Text << " ";
			Comments[i]->Print();
		}
}

void Post::AddLike(Object*& user)
{
	LikedBy[LikesCounter] = user;
	LikesCounter++;
}

Post::Post()
{
	Text = NULL;
	ID = NULL;
	Comments = NULL;
	Author = NULL;
	LikedBy = NULL;
	CommentCounter = 0;
	LikesCounter = 0;
}

Post::~Post()
{
	if (ID)
		delete[] ID;
	if (Text)
		delete[] Text;
	Author = NULL;
	LikedBy = NULL;
	if (Comments)
		for (int i = 0; i < CommentCounter; i++)
			delete Comments[i];
		delete[] Comments;

}

void Post::AggregateComments(Comment*& comment)// add the coming comment in List
{
	if(!CommentCounter)
		Comments = new Comment * [MaxComments];
	Comments[CommentCounter] = comment;
	CommentCounter++;
}

void Post::PrintAll()// Print all the people who liked Post
{
	for (int i = 0; i < LikesCounter ; i++)
		LikedBy[i]->PrintLikes();
}

void Post::Print()
{
	// time check 
	if (Posted_On.LatestCheck())// Print Latest Post along with comments
	{
		Author->PrintName();
		cout << " Shared: " << Text; 
		Posted_On.Get();
		cout << endl;
		for (int i = 0; i < CommentCounter; i++)
			Comments[i]->Print();
		cout << endl;
	}
}

void Post::LoadFromFile(ifstream& File)// loading post file
{
	char temp[80];
	File >> temp;

	ID = Helper::GetStringFromBuffer(temp);

	File.getline(temp, 80);

	int a = 0;//Date
	File >> a;
	Posted_On.SetDate(a);
	File >> a;
	Posted_On.SetMonth(a);
	File >> a;
	Posted_On.SetYear(a);

	File.getline(temp, 80);// to ignore space

	File.getline(temp, 80);
	Text = Helper::GetStringFromBuffer(temp);
}

//___________________________________________________________________________//
class Activity :public Post
{
private:
	int Type;
	char* value;
public:
	Activity();
	~Activity();
	void LoadFromFile(ifstream& File);
	void Print();
};

Activity::Activity()//constructor
{
	Type = 0;
	value = NULL;
}

Activity::~Activity()//destructor
{
	if (value)
		delete[] value;
}

void Activity::LoadFromFile(ifstream& File)// loading posts file
{
	Post::LoadFromFile(File);
	char temp[100];
	File >> Type;
	File.getline(temp, 80);
	value = Helper::GetStringFromBuffer(temp);
}

void Activity::Print()
{
	if (Posted_On.LatestCheck())
	{
		string out;// decide cout depending upon value
		if (Type == 4)
			out = " is Celebrating ";
		if (Type == 3)
			out = " is Making ";
		if (Type == 2)
			out = " is Thinking about ";
		if (Type == 1)
			out = " is Feeling ";

		Author->PrintName();// Prints the activity in particular format along wiith comments
		cout << out << value << endl << Text << "  ";
		Posted_On.Get();
		cout << endl << endl;
		for (int i = 0; i < CommentCounter; i++)
			Comments[i]->Print();
		cout << endl;
	}	
}

//___________________________________________________________________________//
class Page : public Object// Class to keep track of pages
{
private:
	char* Title;
	Post** TimeLine;
	int PostCounter;
public:
	Page();
	~Page();
	void LoadDataPage(ifstream& Filein);// Load Page file
	void PrintTimeline();// Prints the post in timeline
	void PrintName();
	void PrintLatest();// Print latest post
	void PrintLikes();// Print people who liked the post of Page
	int GetPostCounter()	{	return PostCounter;	}// Getters and setters
	char* GetTitle()		{	return Title;		}
	char* getID()			{	return ID;			}
	void SetPosts(Post*& List)
	{
		if(!PostCounter)
			TimeLine = new Post * [MaxLikes];
		TimeLine[PostCounter] = List;
		PostCounter++;
	}

	const Page& operator=(const Page& rhs)// Assignment operator
	{
		if (&rhs != this)	//Avoid self-assignment
		{
			delete[] ID;
			delete[] Title;

			PostCounter = rhs.PostCounter;
			ID = rhs.ID;
			Title = Helper::GetStringFromBuffer(rhs.Title);
			TimeLine = rhs.TimeLine;
			for (int i = 0; i < PostCounter; i++)
				TimeLine[i] = rhs.TimeLine[i];
			
		}
		return *this;
	}

	Page(const Page& rhs)//Copy Constructor
	{
		ID = rhs.ID;
		Title = Helper::GetStringFromBuffer(rhs.Title);
		TimeLine = rhs.TimeLine;
		PostCounter = rhs.PostCounter;
		for (int i = 0; i < PostCounter; i++)
			TimeLine[i] = rhs.TimeLine[i];
	}

};

void Page::PrintLatest()// Print latest post 
{
	for (int i = 0; i < PostCounter; i++)
		TimeLine[i]->Print();
}

void Page::PrintName()// print title of page
{
	cout << Title;
}

void Page::PrintLikes()
{
	cout << ID << " :	" << Title << endl;
}

void Page::PrintTimeline()// Print whole page timeline
{
	for (int i = 0; i < PostCounter; i++)
		TimeLine[i]->PrintTimeline();
}

Page::Page()// Constructor
{
	Title = NULL;
	PostCounter = 0;
	TimeLine = NULL;
}

Page::~Page()//Destructor
{
	if (Title)
		delete[] Title;
	if (TimeLine)
		for (int i = 0; i < PostCounter; i++)
			delete TimeLine[i];
		delete[] TimeLine;
}

void Page::LoadDataPage(ifstream& Filein)// load page file
{
	char buffer[50] = {};
	Filein >> buffer;
	ID = Helper::GetStringFromBuffer(buffer);
	Filein.getline(buffer, 50);
	Title = Helper::GetStringFromBuffer(buffer);
}

//___________________________________________________________________________//
class User :public Object
{
private:
	char* FirstName;
	char* LastName;
	int FCounter;// Number of frineds
	int PCounter;// Number of liked Pages
	int PostCounter;// Total post Shared
	User** FriendList;
	Page** LikedPages;
	Post** TimeLine;
	char tempFriend[40][40][10];// temp for loading file
	char tempPages[40][40][10];
public:
	User();
	~User();
	void LoadDataFromFile(ifstream&, char***& tempflist, char***& tempplist);
	void ViewFriendList();// prints friendlist
	void ViewPageList();// prints all the liked pages
	void ViewHome();// Prints home latest
	void Print();
	void PrintName();
	void PrintLikes();
	void ViewTimeLine();
	const User& operator=(const User& rhs);
	User(const User& rhs);

	//getters and setters
	int GetFcount() { return FCounter; }
	int GetPcount() { return PCounter; }
	char* GetName() { return FirstName; }
	char* GetLastName() { return LastName; }
	char* getID() { return ID; }
	void SetFriends(User**& List) { FriendList = List; }
	void SetPages(Page**& List) { LikedPages = List; }
	void SetPosts(Post*& List)
	{
		if(!PostCounter)
			TimeLine = new Post * [MaxLikes];
		TimeLine[PostCounter] = List;
		PostCounter++;
	}
};

const User& User:: operator=(const User& rhs)// Assignment operator
{
	if (&rhs != this)	//Avoid self-assignment
	{
		delete[] ID;
		delete[] FirstName;
		delete[] LastName;

		ID = rhs.ID;
		FirstName = Helper::GetStringFromBuffer(rhs.FirstName);
		LastName = Helper::GetStringFromBuffer(rhs.LastName);
		FriendList = rhs.FriendList;
		LikedPages = rhs.LikedPages;
		TimeLine = rhs.TimeLine;
		FCounter = rhs.FCounter;
		PCounter = rhs.PCounter;
		PostCounter = rhs.PostCounter;
	}
	return *this;
}

User::User(const User& rhs)//Copy Constructor
{
	ID = rhs.ID;
	FirstName = Helper::GetStringFromBuffer(rhs.FirstName);
	LastName = Helper::GetStringFromBuffer(rhs.LastName);
	FriendList = rhs.FriendList;
	LikedPages = rhs.LikedPages;
	TimeLine = rhs.TimeLine;
	FCounter = rhs.FCounter;
	PCounter = rhs.PCounter;
	PostCounter = rhs.PostCounter;
}

void User::ViewFriendList()
{
	cout << "___________________________________________________________________________" << endl;
	cout << FirstName << " " << LastName << " FriendList" << endl;

	for (int j = 0; j < FCounter; j++)// display friends
		FriendList[j]->Print();
}

void User::PrintLikes()
{
	cout << ID << " :		" << FirstName << " " << LastName << endl;
}

void User::ViewPageList()
{
	cout << "___________________________________________________________________________" << endl;
	cout << FirstName << " " << LastName << " LikedPages " << endl << endl;

	for (int j = 0; j < PCounter; j++)
		LikedPages[j]->PrintLikes();
	cout << "___________________________________________________________________________" << endl;
}

void User::Print()
{
	cout << endl << "User ID :	" << ID << endl;
	cout << "User Name :	" << FirstName << " " << LastName << endl << endl;
}

void User::PrintName()
{
	cout << FirstName << " " << LastName << " ";
}

void User::ViewHome()// shows home 
{
	cout << "___________________________________________________________________________" << endl;
	cout << FirstName << " " << LastName << "  HomePage " << endl << endl;

	for (int i = 0; i < FCounter; i++)
	{
		int count = FriendList[i]->PostCounter;// getting friends total shared posts
		for (int j = 0; j < count; j++)
			FriendList[i]->TimeLine[j]->Print();// calling respective function to print
	}

	cout << "___________________________________________________________________________" << endl;

	for (int i = 0; i < PCounter; i++)
		LikedPages[i]->PrintLatest();// calling respective function to print
	cout << "___________________________________________________________________________" << endl;
}

void User::ViewTimeLine()// prints all posts of current user
{
	cout << "___________________________________________________________________________" << endl;
	cout << FirstName << " " << LastName << " TimeLine " << endl << endl;

	for (int i = 0; i < PostCounter; i++)
		TimeLine[i]->Print(), cout << endl;

	cout << "___________________________________________________________________________" << endl;
}

User::User()// contructor
{
	FirstName = NULL;
	LastName = NULL;
	FriendList = NULL;
	LikedPages = NULL;
	TimeLine = NULL;
	FCounter = 0;
	PCounter = 0;
	PostCounter = 0;
}

User::~User()//destructor
{
	if (FirstName)
		delete[] FirstName;
	if (LastName)
		delete[] LastName;
	if (FriendList)
		delete[] FriendList;
	if (LikedPages)
		delete[] LikedPages;
	if (TimeLine)
	{
		for (int i = 0; i < PostCounter; i++)
			delete TimeLine[i];
		delete[] TimeLine;
	}
}

void User::LoadDataFromFile(ifstream& Filein, char***& tempflist, char***& tempplist)// loading data from file
{
	static int i = 0;

	char tempbuffer[50];// readind ID
	Filein >> tempbuffer;
	ID = Helper::GetStringFromBuffer(tempbuffer);

	Filein >> tempbuffer;// First Name
	FirstName = Helper::GetStringFromBuffer(tempbuffer);

	Filein >> tempbuffer;// Last Name
	LastName = Helper::GetStringFromBuffer(tempbuffer);

	tempflist[i] = new char* [MaxFriend];
	for (int j = 0; j < MaxFriend; j++)
		tempflist[i][j] = new char[MaxFriend];

	tempplist[i] = new char* [MaxPages];
	for (int j = 0; j < MaxPages; j++)
		tempplist[i][j] = new char[MaxPages];

	for (int j = 0; j < 10; j++)// break at -1
	{
		Filein >> tempFriend[i][j];
		if (tempFriend[i][j][0] == '-')
			break;
		FCounter++;
	}

	for (int j = 0; j < FCounter; j++)
		tempflist[i][j] = tempFriend[i][j];

	for (int j = 0; j < 10; j++)//break at -1
	{
		Filein >> tempPages[i][j];
		if (tempPages[i][j][0] == '-')
			break;
		PCounter++;
	}

	for (int j = 0; j < PCounter; j++)
		tempplist[i][j] = tempPages[i][j];
	i++;
}

//___________________________________________________________________________//
class Facebook
{
private:
	User* CurrentUser;
	User** UsersList;
	Page** PageList;
	Post** AllPosts;
	Comment** AllComments;
	int UserTotal;//Total number of users
	int PageTotal;// total number of pages
	int PostTotal;// total posts
	int CommentTotal;// total  comments
public:
	Facebook();
	~Facebook();
	void Run();// Driver function
	void LoadData(char***& tempflist, char***& tempplist);// loads file
	void AssociateFriends(char***& tempflist);// associate
	void AssociatePages(char***& tempplist);
	void SetCurrentUser(const char*);
	Page* SearchPageByID(char* id);// search functions
	User* SearchUserByID(char* id);
	Post* SearchPostByID(char* id);
	void ViewPost(const char* ID);// view post with given id
	void Search(const char* String);// search data related to given string
	void LoadAllComments();// loads all commnets
	void LoadAllPosts();// load all posts
	void PrintLikedList(const char* ID);// print like of posts
	void ViewPage(const char* ID);// view page with given id
	void LikePost(const char* ID);// add like to post
	void AddComment(const char* ID, const char* Text);// add comment to post
	void Help(Object* &Author,char* temp);
	User* GetUser() { return CurrentUser; }// getters 
	User* GetList() { return *UsersList; }
};

void Facebook::ViewPost(const char* ID)
{
	char* temp = const_cast<char*>(ID);
	Post* obj = SearchPostByID(temp);// searches post then call the post function to print

	obj->PrintTimeline();
}

void Facebook::AddComment(const char* ID, const char* Text)
{
	cout << "Add comment " << Text << " to " << ID << endl << endl;
	char* temp = const_cast<char*>(ID);
	Post* Posts = SearchPostByID(temp);// searches post with given id
	int total= CommentTotal;
	total++;

	string s = "c" + std::to_string(total++);// creates id for new comment
	Object* Writer = CurrentUser;
	Posts->AddComment(s.c_str(), Text, Writer);
}

void Facebook::ViewPage(const char* ID)
{
	char* temp = const_cast<char*>(ID);
	Page* page = SearchPageByID(temp);// searches page with given id then call function to print

	page->PrintTimeline();
}

void Facebook::Search(const char* String)
{
	cout << "Searching " << String<<endl<<endl;
	cout << "Users: " << endl;
	for (int i = 0; i < UserTotal; i++)// searching users for given string
	{
		if (strstr(UsersList[i]->GetName(), String))
			UsersList[i]->PrintLikes();
		else if (strstr(UsersList[i]->GetLastName(), String))
			UsersList[i]->PrintLikes();
	}

	cout << "Pages: " << endl;
	for (int i = 0; i < PageTotal; i++)// searching pages for given strings
		if (strstr(PageList[i]->GetTitle(), String))
			PageList[i]->PrintLikes();

	cout << "Post: " << endl;
	for (int i = 0; i < PostTotal; i++)// searching posts for given strings
	{
		if (strstr(AllPosts[i]->GetText(), String))
			AllPosts[i]->Print();
		AllPosts[i]->Search(String);
	}
}

void Facebook::LikePost(const char* ID)
{
	cout << "Like " << ID << endl << endl;
	char* temp = const_cast<char*>(ID);

	Post* post = SearchPostByID(temp);// search post with given id and add liker
	Object* Writer = CurrentUser;

	post->AddLike(Writer);
}

Facebook::Facebook()// default constructor
{
	CurrentUser = NULL;
	UsersList = NULL;
	PageList = NULL;
	AllPosts = NULL;
	UserTotal = 0;
	PageTotal = 0;
	PostTotal = 0;
	CommentTotal = 0;
}

void Facebook::PrintLikedList(const char* ID)
{
	char* temp = const_cast<char*>(ID);
	Post* P = SearchPostByID(temp);// searches post with given id then call function to print the list of all likers

	P->PrintAll();
}

Facebook::~Facebook()//destructor
{
	if (PageList)
	{
		for (int i = 0; i < PageTotal; i++)
			if (PageList[i])
				delete  PageList[i];
		delete[] PageList;
	}

	if (UsersList)
	{
		for (int i = 0; i < UserTotal; i++)
			if (UsersList[i])
				delete UsersList[i];
		delete[] UsersList;
	}

	if(AllPosts)
		delete[] AllPosts;
	if (AllComments)
		delete[] AllComments;
}

void Facebook::LoadData(char***& tempflist, char***& tempplist)// Loading File
{
	ifstream FileFriends;
	FileFriends.open("SocialNetworkUsers.txt");

	FileFriends >> UserTotal;// reading total users

	UsersList = new User * [UserTotal];//initializing
	for (int i = 0; i < UserTotal; i++)
		UsersList[i] = new User;
	tempflist = new char** [UserTotal];
	tempplist = new char** [UserTotal];

	//reading Network File User
	for (int i = 0; i < UserTotal; i++)
		UsersList[i]->LoadDataFromFile(FileFriends, tempflist, tempplist);
	FileFriends.close();


	ifstream FilePages;
	FilePages.open("SocialNetworkPages.txt");

	FilePages >> PageTotal;// reading total pages

	//allocating memory
	PageList = new Page * [PageTotal];
	for (int i = 0; i < PageTotal; i++)
		PageList[i] = new Page;

	//reading Pages File
	for (int i = 0; i < PageTotal; i++)
		PageList[i]->LoadDataPage(FilePages);
	FilePages.close();
}

void Facebook::Help(Object*& Author,char* temp)
{
	if (*temp == 'u')
		Author = SearchUserByID(temp);
	else if (*temp == 'p')
		Author = SearchPageByID(temp);
}

void Facebook::LoadAllPosts()// loading post file
{
	ifstream File;
	File.open("SocialNetworkPosts.txt");
	File >> PostTotal;

	//initializing
	AllPosts = new Post * [PostTotal];

	//Reading
	for (int i = 0; i < PostTotal; i++)
	{
		int Type = 0;
		File >> Type;

		//if type is 1 create post if 2 create activity
		if (Type == 1)
			AllPosts[i] = new Post;
		if (Type == 2)
			AllPosts[i] = new Activity;
		AllPosts[i]->LoadFromFile(File);

		char temp[20];// reading author
		File >> temp;

		Object* Author = NULL;
		Help(Author,temp);

		AllPosts[i]->SetAuthor(Author);// setting author

		Author->SetPosts(AllPosts[i]);// setting posts in timline

		Object** LikedBy = new Object * [MaxLikes];
		int k = 0;

		while (*temp != '-')//Liked by
		{
			File >> temp;
			if (*temp == '-')
				break;
			Help(LikedBy[k], temp);// creates object depending upon data
			k++;
		}
		AllPosts[i]->SetLikes(k);// setting total likes and liked list
		AllPosts[i]->SetLikers(LikedBy);
	}
	File.close();
}

void Facebook::LoadAllComments()
{
	ifstream File;
	File.open("SocialNetworkComments.txt");
	File >> CommentTotal;
	AllComments = new Comment * [CommentTotal];
	for (int i = 0; i < CommentTotal; i++)
	{
		AllComments[i] = new Comment;
		char temp[80] = {};

		File >> temp;
		AllComments[i]->SetId(temp);

		File >> temp;// on post id

		Post* post = SearchPostByID(temp);

		if (post)
			post->AggregateComments(AllComments[i]);

		File >> temp;// writer
		Object* Commentby = NULL;

		Help(Commentby, temp);// creating object depending upon data

		AllComments[i]->SetAuthor(Commentby);

		File.getline(temp, 80);//text
		AllComments[i]->SetText(temp);
	}
}

void Facebook::SetCurrentUser(const char* ID)// setter for current user
{
	char* temp = const_cast<char*>(ID);
	CurrentUser = SearchUserByID(temp);//Find user in list and set current
}

Post* Facebook::SearchPostByID(char* id)
{
	for (int i = 0; i < PostTotal; i++)
		if (strcmp(AllPosts[i]->GetID(), id) == 0)
			return AllPosts[i];
}

Page* Facebook::SearchPageByID(char* id)// search the page by id
{
	for (int m = 0; m < PageTotal; m++)
		if (strcmp(PageList[m]->getID(), id) == 0)
			return PageList[m];
}

User* Facebook::SearchUserByID(char* id)// search user by id
{
	for (int m = 0; m < UserTotal; m++)
		if (strcmp(UsersList[m]->getID(), id) == 0)
			return UsersList[m];
}

void Facebook::AssociateFriends(char***& tempflist)// friend association
{
	for (int i = 0; i < UserTotal; i++)// Running equal to total number of users
	{
		int Friends = UsersList[i]->GetFcount();// gets friend count
		User** Obj = NULL;
		Obj = new User * [Friends];

		int count = 0;
		char arraya[5] = {};

		for (int j = 0; j < Friends; j++)
		{
			for (int k = 0, a = 0; k < MaxFriend; k++, a++)
				if (tempflist[i][j][k] != '\0')
					arraya[a] = tempflist[i][j][k];

			Obj[count] = SearchUserByID(arraya);// Search friends in the list
			count++;
		}
		UsersList[i]->SetFriends(Obj);// setting
	}

	for (int m = 0; m < UserTotal  ; m++)
	{
		/*for (int n = 0 ; n < MaxFriend ; n++)
				delete [] tempflist [m][n];*/
		delete [] tempflist[m];
	}
	delete[] tempflist;
}

void Facebook::AssociatePages(char***& tempplist)//Page Association
{
	for (int i = 0; i < PageTotal; i++)// Running equal to total number of pages
	{
		int Pages = UsersList[i]->GetPcount();// get liked pages count
		Page** Obj = NULL;
		Obj = new Page * [Pages];
		int count = 0;
		char arraya[5] = {};

		for (int j = 0; j < Pages; j++)
		{
			for (int k = 0, a = 0; k < MaxPages; k++, a++)
				if (tempplist[i][j][k] != '\0')
					arraya[a] = tempplist[i][j][k];

			Obj[count] = SearchPageByID(arraya);
			count++;
		}
		UsersList[i]->SetPages(Obj);//setter
	}

	for (int m = 0; m < UserTotal; m++)
	{
		/*for (int n = 0; n < MaxPages; n++)
			delete[] tempplist [m][n];*/
		delete[] tempplist[m];
	}
	delete[] tempplist;

}

void Facebook::Run()//run function
{
	system("color ED");
	char*** tempFriendList = NULL;
	char*** tempLikedPages = NULL;

	Facebook FB;
	LoadData(tempFriendList, tempLikedPages);
	LoadAllPosts();
	LoadAllComments();

	AssociateFriends(tempFriendList);
	AssociatePages(tempLikedPages);

	cout << "Command:	Set Current User" << endl;
	cout << "___________________________________________________________________________" << endl;

	SetCurrentUser("u7");
	GetUser()->Print();
	cout << "Set as Current User" << endl;
	cout << "___________________________________________________________________________" << endl;

	cout << "Command:	Set Current Date" << endl;
	cout << "___________________________________________________________________________" << endl;
	Date::Current_Date.setDate(15, 11, 2017);
	cout << "Current Date:	"; 
	Date::Current_Date.Get(); cout << endl;
	cout << "___________________________________________________________________________" << endl;

	cout << "Command:	View Friend List" << endl;
	GetUser()->ViewFriendList();

	cout << "___________________________________________________________________________" << endl;
	cout << "Command:	View Liked Pages" << endl;
	GetUser()->ViewPageList();

	cout << "Command:	View Home Page" << endl;
	GetUser()->ViewHome();

	cout << "Command:	View TimeLine" << endl;
	GetUser()->ViewTimeLine();

	cout << "Command:	View LikedBy" << endl;
	cout << "___________________________________________________________________________" << endl;
	PrintLikedList("post5");
	cout << "___________________________________________________________________________" << endl;

	cout << "Command:	View Page" << endl;
	cout << "___________________________________________________________________________" << endl;
	ViewPage("p1");
	cout << "___________________________________________________________________________" << endl;

	cout << "Command:	Search" << endl;
	cout << "___________________________________________________________________________" << endl;
	Search("Ali");
	cout << "___________________________________________________________________________" << endl;
	cout << "Command:	Search" << endl;
	cout << "___________________________________________________________________________" << endl;
	Search("Birthday");
	cout << "___________________________________________________________________________" << endl;

	cout << "Command:	LikePost" << endl;
	cout << "___________________________________________________________________________" << endl;
	LikePost("post5");
	PrintLikedList("post5");

	cout << "___________________________________________________________________________" << endl;
	cout << "Command:	Post Comment" << endl;
	cout << "___________________________________________________________________________" << endl;
	AddComment("post4", "Good Luck Buddy");
	ViewPost("post4");
	cout << "___________________________________________________________________________" << endl;

	cout << "Command:	Post Comment" << endl;
	cout << "___________________________________________________________________________" << endl;
	AddComment("post8", "Thanks for the wishes");
	ViewPost("post8");
	cout << "___________________________________________________________________________" << endl;
}

void main()
{
	Facebook FB;
	FB.Run();
}
