/**
 * 学生成绩管理系统 — WebAssembly 核心计算模块
 * 将 C++ 核心算法编译为 .wasm 供浏览器调用
 *
 * 新增接口（v2）：
 *   - update_student    修改学生记录
 *   - set_comment       设置教师评语
 *   - get_comment       获取教师评语
 *
 * 编译（Emscripten）：
 *   em++ student_system_wasm.cpp -o student_system.js \
 *     -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -O2 --no-entry \
 *     -s EXPORTED_FUNCTIONS='[...]' \
 *     -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString","stringToUTF8"]'
 */

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif

using namespace std;

constexpr int MAX_STUDENTS = 30;
constexpr int MAX_COURSES  = 6;

struct Student {
    long long num = 0;
    string name;
    vector<double> score;
    double sum = 0.0;
    double aver = 0.0;
    string comment;   // v2: 教师评语
};

// 全局状态
static vector<Student> g_students;
static int g_n = 0;
static int g_m = 0;

// ==================== 核心算法 ====================

void computeOneStudent(Student& s) {
    s.sum = accumulate(s.score.begin(), s.score.end(), 0.0);
    s.aver = s.score.empty() ? 0.0 : s.sum / static_cast<double>(s.score.size());
}

void computeAllStudents(vector<Student>& stus) {
    for (auto& s : stus) computeOneStudent(s);
}

vector<Student> makeRankList(vector<Student> stus) {
    computeAllStudents(stus);
    sort(stus.begin(), stus.end(), [](const Student& a, const Student& b) {
        if (a.sum != b.sum) return a.sum > b.sum;
        return a.num < b.num;
    });
    return stus;
}

int findRankByNum(const vector<Student>& stus, long long num) {
    auto ranked = makeRankList(stus);
    for (size_t i = 0; i < ranked.size(); ++i)
        if (ranked[i].num == num) return static_cast<int>(i) + 1;
    return -1;
}

// ==================== JSON 辅助 ====================

static string jsonEscape(const string& s) {
    string r;
    for (char c : s) {
        switch (c) {
        case '"': r += "\\\""; break;
        case '\\': r += "\\\\"; break;
        default: r += c;
        }
    }
    return r;
}

static string fmtDouble(double v) {
    ostringstream oss;
    oss << fixed << setprecision(2) << v;
    return oss.str();
}

static string studentToJson(const Student& s, int rank) {
    ostringstream j;
    j << "{\"num\":" << s.num << ",\"name\":\"" << jsonEscape(s.name)
      << "\",\"scores\":[";
    for (size_t i = 0; i < s.score.size(); ++i) {
        if (i) j << ",";
        j << fmtDouble(s.score[i]);
    }
    j << "],\"sum\":" << fmtDouble(s.sum) << ",\"aver\":" << fmtDouble(s.aver)
      << ",\"rank\":" << rank << ",\"comment\":\"" << jsonEscape(s.comment) << "\"}";
    return j.str();
}

static char* allocString(const string& s) {
    char* buf = static_cast<char*>(malloc(s.size() + 1));
    if (buf) memcpy(buf, s.c_str(), s.size() + 1);
    return buf;
}

// ==================== 导出接口 ====================

extern "C" {

EMSCRIPTEN_KEEPALIVE void init_system() { g_students.clear(); g_n = g_m = 0; }

EMSCRIPTEN_KEEPALIVE int add_student(long long num, const char* name,
                                      const double* scores, int sc_count) {
    if (g_n >= MAX_STUDENTS) return -1;
    if (g_n > 0 && sc_count != g_m) return -2;
    Student s;
    s.num = num; s.name = string(name);
    s.score.assign(scores, scores + sc_count);
    computeOneStudent(s);
    if (g_m == 0) g_m = sc_count;
    g_students.push_back(s); g_n = static_cast<int>(g_students.size());
    return 0;
}

// v2: 更新学生
EMSCRIPTEN_KEEPALIVE int update_student(int idx, long long num, const char* name,
                                         const double* scores, int sc_count) {
    if (idx < 0 || idx >= g_n) return -1;
    auto& s = g_students[idx];
    s.num = num; s.name = string(name);
    s.score.assign(scores, scores + sc_count);
    computeOneStudent(s);
    return 0;
}

// v2: 评语
EMSCRIPTEN_KEEPALIVE int set_comment(int idx, const char* comment) {
    if (idx < 0 || idx >= g_n) return -1;
    g_students[idx].comment = string(comment ? comment : "");
    return 0;
}

EMSCRIPTEN_KEEPALIVE char* get_comment(int idx) {
    if (idx < 0 || idx >= g_n) return allocString("");
    return allocString(g_students[idx].comment);
}

EMSCRIPTEN_KEEPALIVE int get_student_count() { return g_n; }
EMSCRIPTEN_KEEPALIVE int get_course_count() { return g_m; }

EMSCRIPTEN_KEEPALIVE char* get_all_students_json() {
    computeAllStudents(g_students);
    auto ranked = makeRankList(g_students);
    ostringstream j; j << "[";
    for (size_t i = 0; i < g_students.size(); ++i) {
        if (i) j << ",";
        int rank = -1;
        for (size_t r = 0; r < ranked.size(); ++r)
            if (ranked[r].num == g_students[i].num) { rank = static_cast<int>(r) + 1; break; }
        j << studentToJson(g_students[i], rank);
    }
    j << "]";
    return allocString(j.str());
}

EMSCRIPTEN_KEEPALIVE void sort_by_score_desc() {
    if (g_students.empty()) return;
    computeAllStudents(g_students);
    sort(g_students.begin(), g_students.end(), [](const Student& a, const Student& b) {
        if (a.sum != b.sum) return a.sum > b.sum;
        return a.num < b.num;
    });
}

EMSCRIPTEN_KEEPALIVE void sort_by_score_asc() {
    if (g_students.empty()) return;
    computeAllStudents(g_students);
    sort(g_students.begin(), g_students.end(), [](const Student& a, const Student& b) {
        if (a.sum != b.sum) return a.sum < b.sum;
        return a.num < b.num;
    });
}

EMSCRIPTEN_KEEPALIVE void sort_by_num() {
    sort(g_students.begin(), g_students.end(),
         [](const Student& a, const Student& b) { return a.num < b.num; });
}

EMSCRIPTEN_KEEPALIVE void sort_by_name() {
    sort(g_students.begin(), g_students.end(), [](const Student& a, const Student& b) {
        if (a.name != b.name) return a.name < b.name;
        return a.num < b.num;
    });
}

EMSCRIPTEN_KEEPALIVE char* search_by_num_json(long long target) {
    computeAllStudents(g_students);
    for (const auto& s : g_students) {
        if (s.num == target) {
            int rank = findRankByNum(g_students, s.num);
            return allocString(studentToJson(s, rank));
        }
    }
    return allocString("null");
}

EMSCRIPTEN_KEEPALIVE char* search_by_name_json(const char* target) {
    computeAllStudents(g_students);
    string t(target);
    ostringstream j; j << "["; bool first = true;
    for (const auto& s : g_students) {
        if (s.name == t) {
            if (!first) j << ",";
            int rank = findRankByNum(g_students, s.num);
            j << studentToJson(s, rank);
            first = false;
        }
    }
    j << "]";
    return allocString(j.str());
}

EMSCRIPTEN_KEEPALIVE char* get_course_stats_json() {
    computeAllStudents(g_students);
    ostringstream j; j << "[";
    for (int i = 0; i < g_m; ++i) {
        if (i) j << ","; double sum = 0.0;
        for (const auto& s : g_students) sum += s.score[i];
        j << "{\"courseIndex\":" << (i+1) << ",\"total\":" << fmtDouble(sum)
          << ",\"average\":" << fmtDouble(g_n > 0 ? sum/g_n : 0.0) << "}";
    }
    j << "]";
    return allocString(j.str());
}

EMSCRIPTEN_KEEPALIVE char* get_distribution_json(int course_index) {
    if (course_index < 0 || course_index >= g_m || g_n == 0)
        return allocString("null");
    int cnt[6] = {0,0,0,0,0,0};
    const char* labels[] = {"<60","60-69","70-79","80-89","90-99","100"};
    for (const auto& s : g_students) {
        double sc = s.score[course_index];
        if (sc < 60.0) cnt[0]++; else if (sc < 70.0) cnt[1]++;
        else if (sc < 80.0) cnt[2]++; else if (sc < 90.0) cnt[3]++;
        else if (sc < 100.0) cnt[4]++; else cnt[5]++;
    }
    ostringstream j;
    j << "{\"courseIndex\":" << (course_index+1) << ",\"distribution\":[";
    for (int i = 0; i < 6; ++i) {
        if (i) j << ",";
        double pct = (g_n > 0) ? (100.0 * cnt[i] / g_n) : 0.0;
        j << "{\"range\":\"" << labels[i] << "\",\"count\":" << cnt[i]
          << ",\"percentage\":" << fmtDouble(pct) << "}";
    }
    j << "]}";
    return allocString(j.str());
}

EMSCRIPTEN_KEEPALIVE int remove_student(int idx) {
    if (idx < 0 || idx >= g_n) return -1;
    g_students.erase(g_students.begin() + idx);
    g_n = static_cast<int>(g_students.size());
    if (g_n == 0) g_m = 0;
    return 0;
}

EMSCRIPTEN_KEEPALIVE void clear_all() { g_students.clear(); g_n = g_m = 0; }

EMSCRIPTEN_KEEPALIVE void free_string(char* ptr) { free(ptr); }

}  // extern "C"
