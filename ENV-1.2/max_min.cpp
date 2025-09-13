#include <iostream>

bool isNumber(const std::string& s, int& num) {
  if(s.empty()) {
     return false;
  }
  for(char c : s) {
     if(!isdigit(c)) {
	 return false;
    }
  }   
  num = stoi(s);
  return true;  
}

int main(int argc, char* argv[]) {
  int a, b, c;	
  if(argc != 4 || !isNumber(argv[1], a) || !isNumber(argv[2], b) || !isNumber(argv[3], c)){
     std::cout << "Usage: ./program a b c\n";
     return 1;	 
  }	

  int max = a;
  int min = a;

  if(b > max) max = b;
  if(c > max) max = c;

  if(b < min) min = b;
  if(c < min) min = c;

  std::cout << "min-" << min << ", max-" << max << "\n";
  return 0;
}	

