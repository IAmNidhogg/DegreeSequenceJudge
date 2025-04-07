#include <fcntl.h>
#include <iostream>
#include <matplot/matplot.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <utility>

bool judge_if_graph_seq(std::vector<int> &seq);
void bubble_sort(std::vector<int> &seq);
bool equal_to_zero(std::vector<int> &seq);
bool non_negative(std::vector<int> &seq);
int kbhit(void);

int main() {
  using namespace matplot;
  std::vector<int> seq;
  std::vector<int> seq_to_draw;
  std::vector<std::pair<size_t, size_t>> graph_edges;

  int value;

  std::cout << "请输入度序列 ：（以空格分隔）" << std::endl;

  do {
    std::cin >> value;
    seq.push_back(value);
  } while (value != 999);
  seq.pop_back();

  seq_to_draw = seq;

  for (size_t index = 0; index < seq.size(); ++index) {
    std::cout << seq[index] << " ";
  }
  std::cout << std::endl;

  value = 0;

  for (size_t index = 0; index < seq.size(); ++index) {
    value += seq[index];
  }
  std::cout << "The sum of seq is " << value << std::endl;

  if (value % 2 != 0) {
    std::cout << "不是度序列" << std::endl;
    return 0;
  } else {
    std::cout << "是度序列" << std::endl;
  }

  if (value % 2 == 0) {
    bool if_graph_seq = judge_if_graph_seq(seq);
    if (if_graph_seq) {
      std::cout << "是图序列" << std::endl;
    } else {
      return 0;
    }
  }

  for (size_t index = 0; index < seq_to_draw.size(); ++index) {
    size_t index_n = index + 1;
    while (seq_to_draw[index] != 0) {
      if (seq_to_draw[index_n] == 0) {
        ++index_n;
        continue;
      }
      graph_edges.push_back(std::make_pair(index, index_n));
      --seq_to_draw[index];
      --seq_to_draw[index_n];
      ++index_n;
    }
  }

  graph(graph_edges);
  show();

  while (1) {
    int key = kbhit();
    if (key == 1) {
      break;
    }
  }

  return 0;
}

bool judge_if_graph_seq(std::vector<int> &seq) {
  bubble_sort(seq);

  if (equal_to_zero(seq)) {
    return true;
  } else if (seq.size() - 1 >= seq[0]) {
    int mark = seq[0];
    for (size_t index = 0; index < mark + 1; ++index) {
      seq[index] = seq[index] - 1;
    }
    seq.erase(seq.begin());

    if (non_negative(seq)) {
      judge_if_graph_seq(seq);
    } else {
      std::cout << "不是图序列" << std::endl;
      return false;
    }
  } else {
    std::cout << "不是图序列" << std::endl;
    return false;
  }

  return false;
}

void bubble_sort(std::vector<int> &seq) {
  for (size_t index = 0; index < seq.size() - 1; ++index) {
    for (size_t index_inside = 0; index_inside < seq.size() - index - 1;
         ++index_inside) {
      if (seq[index_inside] < seq[index_inside + 1]) {
        int tmp = seq[index_inside + 1];
        seq[index_inside + 1] = seq[index_inside];
        seq[index_inside] = tmp;
      }
    }
  }
}

bool equal_to_zero(std::vector<int> &seq) {
  for (size_t index = 0; index < seq.size(); ++index) {
    if (seq[index] != 0) {
      return false;
    }
  }
  return true;
}

bool non_negative(std::vector<int> &seq) {
  for (size_t index = 0; index < seq.size(); ++index) {
    if (seq[index] < 0) {
      return false;
    }
  }
  return true;
}

// Determine a keyboard is hit or not. If yes, return 1. If not, return 0.
int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);

  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF) {
    ungetc(ch, stdin);
    // fflush(stdin);
    return 1;
  }
  return 0;
}
