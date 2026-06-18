/**
 * 学生成绩管理系统 — 核心算法单元测试
 * 运行: npm test  或  npx jest
 */
const { describe, test, expect } = require('@jest/globals');

// 从 index.html 提取引擎核心逻辑进行测试
// 此处独立实现核心算法以验证正确性

function computeOneStudent(s) {
  let sum = 0;
  for (let i = 0; i < s.scores.length; i++) sum += s.scores[i];
  s.sum = sum;
  s.aver = s.scores.length > 0 ? sum / s.scores.length : 0;
}

function makeRankList(students) {
  const list = [...students];
  list.sort((a, b) => {
    if (a.sum !== b.sum) return b.sum - a.sum;
    return a.num - b.num;
  });
  return list;
}

function scoreToGP(s) {
  if (s >= 90) return 4.0;
  if (s >= 85) return 3.7;
  if (s >= 82) return 3.3;
  if (s >= 78) return 3.0;
  if (s >= 75) return 2.7;
  if (s >= 72) return 2.3;
  if (s >= 68) return 2.0;
  if (s >= 64) return 1.5;
  if (s >= 60) return 1.0;
  return 0;
}

function calcGPA(scores, credits) {
  let tp = 0, tc = 0;
  for (let i = 0; i < scores.length; i++) {
    tp += scoreToGP(scores[i]) * credits[i];
    tc += credits[i];
  }
  return tc > 0 ? +(tp / tc).toFixed(2) : 0;
}

// ==================== 测试用例 ====================

describe('computeOneStudent', () => {
  test('正确计算总分和平均分', () => {
    const s = { scores: [80, 90, 70] };
    computeOneStudent(s);
    expect(s.sum).toBe(240);
    expect(s.aver).toBe(80);
  });

  test('空成绩数组', () => {
    const s = { scores: [] };
    computeOneStudent(s);
    expect(s.sum).toBe(0);
    expect(s.aver).toBe(0);
  });

  test('单科满分', () => {
    const s = { scores: [100] };
    computeOneStudent(s);
    expect(s.sum).toBe(100);
    expect(s.aver).toBe(100);
  });
});

describe('makeRankList', () => {
  test('按总分降序排名', () => {
    const students = [
      { num: 1, name: 'A', scores: [80], sum: 80 },
      { num: 2, name: 'B', scores: [90], sum: 90 },
      { num: 3, name: 'C', scores: [70], sum: 70 },
    ];
    const ranked = makeRankList(students);
    expect(ranked[0].num).toBe(2); // 90分
    expect(ranked[1].num).toBe(1); // 80分
    expect(ranked[2].num).toBe(3); // 70分
  });

  test('同分按学号升序', () => {
    const students = [
      { num: 3, name: 'C', scores: [80], sum: 80 },
      { num: 1, name: 'A', scores: [80], sum: 80 },
      { num: 2, name: 'B', scores: [80], sum: 80 },
    ];
    const ranked = makeRankList(students);
    expect(ranked[0].num).toBe(1);
    expect(ranked[1].num).toBe(2);
    expect(ranked[2].num).toBe(3);
  });

  test('空列表', () => {
    expect(makeRankList([])).toEqual([]);
  });
});

describe('scoreToGP', () => {
  test('各分段绩点正确', () => {
    expect(scoreToGP(95)).toBe(4.0);
    expect(scoreToGP(87)).toBe(3.7);
    expect(scoreToGP(83)).toBe(3.3);
    expect(scoreToGP(80)).toBe(3.0);
    expect(scoreToGP(76)).toBe(2.7);
    expect(scoreToGP(73)).toBe(2.3);
    expect(scoreToGP(70)).toBe(2.0);
    expect(scoreToGP(65)).toBe(1.5);
    expect(scoreToGP(62)).toBe(1.0);
    expect(scoreToGP(50)).toBe(0);
  });

  test('边界值', () => {
    expect(scoreToGP(90)).toBe(4.0);
    expect(scoreToGP(60)).toBe(1.0);
    expect(scoreToGP(59)).toBe(0);
    expect(scoreToGP(0)).toBe(0);
    expect(scoreToGP(100)).toBe(4.0);
  });
});

describe('calcGPA', () => {
  const credits = [5, 4, 4, 4, 3, 2, 2];

  test('正常计算GPA', () => {
    const scores = [88, 92, 78, 85, 90, 80, 75];
    // 绩点: 3.3, 4.0, 3.0, 3.7, 4.0, 3.0, 2.7
    // 加权: 3.3*5 + 4.0*4 + 3.0*4 + 3.7*4 + 4.0*3 + 3.0*2 + 2.7*2
    // = 16.5 + 16 + 12 + 14.8 + 12 + 6 + 5.4 = 82.7
    // 总学分 = 24
    // GPA = 82.7 / 24 = 3.45
    const gpa = calcGPA(scores, credits);
    expect(gpa).toBeCloseTo(3.45, 1);
  });

  test('全满分GPA=4.0', () => {
    const scores = [100, 100, 100, 100, 100, 100, 100];
    expect(calcGPA(scores, credits)).toBe(4.0);
  });

  test('全不及格GPA=0', () => {
    const scores = [50, 50, 50, 50, 50, 50, 50];
    expect(calcGPA(scores, credits)).toBe(0);
  });
});

describe('排序算法', () => {
  test('按总分降序', () => {
    const arr = [
      { num: 1, sum: 70 }, { num: 2, sum: 90 }, { num: 3, sum: 80 }
    ];
    arr.sort((a, b) => {
      if (a.sum !== b.sum) return b.sum - a.sum;
      return a.num - b.num;
    });
    expect(arr.map(s => s.num)).toEqual([2, 3, 1]);
  });

  test('按学号升序', () => {
    const arr = [
      { num: 3, sum: 80 }, { num: 1, sum: 90 }, { num: 2, sum: 70 }
    ];
    arr.sort((a, b) => a.num - b.num);
    expect(arr.map(s => s.num)).toEqual([1, 2, 3]);
  });
});
