#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>

struct ThreadData {
  const int* arr;
  size_t start;
  size_t end;
  long long partialSum;
};

void* sumArrayPart(void* arg) {
  ThreadData* data = (ThreadData*)arg;
  long long sum = 0;
  for (size_t i = data->start; i < data->end; ++i) {
    sum += data->arr[i];
  }
  data->partialSum = sum;
  return nullptr;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <N> <M>\n";
    return 1;
  }

  size_t N = std::stoi(argv[1]); 
  size_t M = std::stoi(argv[2]); 

  if (N < 1000000) {
    std::cerr << "N must be greater than 1.000.000\n";
    return 1;
  }
  if (M <= 0) {
    std::cerr << "M must be at least 1\n";
    return 1;
  }

  std::vector<int> arr(N);
  std::srand(std::time(nullptr));
  for (size_t i = 0; i < N; ++i) {
    arr[i] = std::rand() % 100 + 1; 
  }

  auto startSingle = std::chrono::high_resolution_clock::now();

  long long singleSum = 0;
  for (size_t i = 0; i < N; ++i) {
    singleSum += arr[i];
  }

  auto endSingle = std::chrono::high_resolution_clock::now();
  double singleDuration = std::chrono::duration<double>(endSingle - startSingle).count();

  std::vector<pthread_t> threads(M);
  std::vector<ThreadData> threadData(M);

  size_t chunkSize = N / M;

  auto startMulti = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < M; ++i) {
    threadData[i].arr = arr.data();
    threadData[i].start = i * chunkSize;
    threadData[i].end = (i == M - 1) ? N : (i + 1) * chunkSize;
    pthread_create(&threads[i], nullptr, sumArrayPart, &threadData[i]);
  }

  long long totalSum = 0;
  for (size_t i = 0; i < M; ++i) {
    pthread_join(threads[i], nullptr);
    totalSum += threadData[i].partialSum;
  }

  auto endMulti = std::chrono::high_resolution_clock::now();
  double multiDuration = std::chrono::duration<double>(endMulti - startMulti).count();

  std::cout << "Time spent without threads: " << singleDuration << "\n";
  std::cout << "Time spent with " << M << " threads: " << multiDuration << "\n";

  return 0;
}
