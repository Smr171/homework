# ============================================================
# 学生成绩管理系统 — Emscripten WebAssembly 编译
# 前置: https://emscripten.org/docs/getting_started/downloads.html
# ============================================================

CXX = em++
CXXFLAGS = -O2 -Wall

EXPORTS = _malloc _free _init_system _add_student _update_student \
          _set_comment _get_comment _get_student_count _get_course_count \
          _get_all_students_json _sort_by_score_desc _sort_by_score_asc \
          _sort_by_num _sort_by_name _search_by_num_json _search_by_name_json \
          _get_course_stats_json _get_distribution_json _remove_student \
          _clear_all _free_string

EMFLAGS = -s WASM=1 \
          -s EXPORTED_FUNCTIONS='[$(addprefix ",$(addsuffix ",$(EXPORTS)))]' \
          -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString","stringToUTF8","lengthBytesUTF8"]' \
          -s ALLOW_MEMORY_GROWTH=1 --no-entry

.PHONY: all clean

all: student_system.js

student_system.js: student_system_wasm.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(EMFLAGS)
	@echo "============================================"
	@echo "  WebAssembly 编译完成!"
	@echo "  - student_system.js   (JS 胶水代码)"
	@echo "  - student_system.wasm (WASM 模块)"
	@echo "============================================"

clean:
	rm -f student_system.js student_system.wasm
	@echo "已清理"
