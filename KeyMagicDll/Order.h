#ifndef ORDER_H
#define ORDER_H

#define CCL 1
#define NOT 2
#define CHR 3
#define SET 4
#define END 5

class Order{
public:
	Order(){
		return;
	};
	~Order(){
		return;
	};

	bool Pattern(const wchar_t* pattern, int length);
	bool Match(const wchar_t* Dest, int length);
	wchar_t order(wchar_t* Dest);

private:
	void MakeSet(const wchar_t Start,const wchar_t End);
	void MakeSet(const wchar_t New);
	bool IsInSet(const wchar_t Input);

	wchar_t CPattern[2048];
	UINT cPatternLen;
	wchar_t* Set[20];
	wchar_t* Address;
	wchar_t SetStart;
	wchar_t SetEnd;
};

#endif
