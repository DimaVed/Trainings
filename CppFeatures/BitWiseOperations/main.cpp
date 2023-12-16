#include <cmath>
#include <assert.h>


bool GetNthtBit(char c, size_t n) {

	return (c >> n) & 1u;
}

//bool GetFirstBit( char c) { return true; }

//bool GetLastBit(char c) { return true; }


void SetNthBit(char& x, size_t n, bool value) {
  if (value) {
    x = x | (1u << n);
  } else {
    x = x | ~(1u << n);
  }

}

template <typename T>
void Swap1(T& x, T& y) {
  x = x + y;
  y = x - y;
  x = x - y;
}
struct Point {
  int x;
  int y;
};


template <typename T>
void Swap2(T& x, T& y) {
  x = x ^ y;
  y = x ^ y;
  x = x ^ y;
}



int main( )
{
  auto ByteSize = sizeof(std::byte);
  auto CharSize = sizeof(char);
  auto ShortSize = sizeof(short);
  auto IntSize = sizeof(int);
  auto Size_tSize = sizeof(size_t);
  size_t x = 3;
  size_t y = 5;
  Swap1(x, y);
  Swap2(x, y);
  Point p1{1, 1};
  Point p2{2, 2};
 // Swap2(p1, p2); // Ошибка компиляции
  double d1 = 1.;
  double d2 = 2.;
  d1 = d1 << 1;
  //Swap2(d1, d2);


  char a = 0x00;
  a = 0x01;
  a =  a << 1;
  SetNthBit(a, 0, true);

  a = 2;
  char b = 3;

  auto val  = a^b;
  
  




	return 0;
}
