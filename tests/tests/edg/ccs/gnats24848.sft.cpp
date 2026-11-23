//type:fp
//options_all:--c++11 --il_display
//filter:grep initializer_range -A4 | edg-normalize-test-output

int foo();

class C
{
public:
        C()
        {
        }
        C(int i)
        {
        }
private:
   int l = 10;
   int m = foo();
   int n = 10;
};
