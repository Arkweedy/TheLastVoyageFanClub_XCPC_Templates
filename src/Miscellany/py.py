def RandomAndList():
    """
    关于随机数和列表操作的常用技巧。
    """
    import random

    # 生成一个符合正态分布（高斯分布）的随机浮点数。
    # 第一个参数是均值 (mu)，第二个参数是标准差 (sigma)。
    # 类似于 C++ 中使用 <random> 库的 std::normal_distribution。
    random.normalvariate(0.5, 0.1)

    # 列表推导式：一种非常 Pythonic 的创建列表的方式。
    # 这里创建了一个包含字符串 '0' 到 '8' 的列表。
    # 注意：l 中的元素是 str 类型，不是 int 类型。
    l = [str(i) for i in range(9)]

    # sorted(l): 返回一个新的已排序的列表，原列表 l 不变。由于元素是字符串，所以是按字典序排序。
    # min(l), max(l): 同样按字典序返回最小/最大元素。
    # len(l): 返回列表长度，相当于 C++ 中的 a.size()。
    sorted(l), min(l), max(l), len(l)

    # random.shuffle(l): 原地打乱列表 l 中的元素顺序。
    # 相当于 C++ 中的 std::shuffle。
    random.shuffle(l)
    
    int_l = [i for i in range(9)]
    
    # l.sort(): 原地排序列表。
    # key=lambda x: x ^ 1: `key` 参数接受一个函数，该函数为列表中的每个元素计算一个用于排序的“键”。
    # `lambda x: ...` 是一个匿名函数，这里表示对于每个元素 x，使用 x ^ 1 的结果进行排序。
    # reverse=True: 表示降序排序。
    int_l.sort(key=lambda x: x ^ 1, reverse=True)

    from functools import cmp_to_key
    # 在 Python 3 中，sort 不再支持 cmp 函数 (即比较两个元素的函数)。
    # cmp_to_key 可以将一个老式的 cmp 函数 (返回负、零、正) 转换成一个 key 函数。
    # 这对于习惯了 C++ `sort` 中自定义比较器的选手很有帮助。
    # `lambda x, y: (y^1) - (x^1)` 就是一个 cmp 函数。
    int_l.sort(key=cmp_to_key(lambda x, y: (y ^ 1) - (x ^ 1)))


    # itertools 模块：包含了一系列用于高效迭代的工具，功能非常强大。
    from itertools import *

    # product: 计算笛卡尔积。相当于 C++ 中的多层嵌套 for 循环。
    # product('ABCD', repeat=2) 会生成 AA AB AC AD BA BB BC BD...
    for i in product('ABCD', repeat=2):
        pass 
    
    # permutations: 生成指定长度的全排列。
    # permutations('ABCD', 2) 会生成 AB AC AD BA BC BD CA CB... (P(4, 2))
    for i in permutations('ABCD', 2):
        pass

    # combinations: 生成指定长度的组合（不考虑顺序）。
    # combinations('ABCD', 2) 会生成 AB AC AD BC BD CD (C(4, 2))
    for i in combinations('ABCD', 2):
        pass

    # combinations_with_replacement: 生成可重复选择的组合。
    # combinations_with_replacement('ABCD', 2) 会生成 AA AB AC AD BB BC BD CC CD DD
    for i in combinations_with_replacement('ABCD', 2):
        pass

def FractionOperation():
    """
    分数（有理数）运算，可以避免浮点数精度问题。
    """
    from fractions import Fraction
    
    # 示例分数
    a = Fraction(3, 4) # a = 3/4
    
    # a.numerator: 获取分子
    # a.denominator: 获取分母
    # str(a): 将分数转换为字符串 "3/4"
    print(a.numerator, a.denominator, str(a))

    # 一个非常有用的功能：将一个浮点数转换为最接近它的、分母不超过指定值的分数。
    # 这在处理一些需要精确分数的几何或数论问题时非常有用。
    a = Fraction(0.233).limit_denominator(1000) # 结果会是 7/30

def DecimalOperation():
    """
    高精度十进制数运算。这是 Python 在算法竞赛中替代 C++ 高精度写法的核心。
    """
    from decimal import Decimal, getcontext, FloatOperation

    # 设置全局精度（有效数字位数）。
    getcontext().prec = 100

    # 设置舍入模式，默认是 ROUND_HALF_EVEN (四舍六入五成双)。
    # 其他模式如 ROUND_FLOOR (向下舍入), ROUND_CEILING (向上舍入) 等。
    # import decimal # 需要先导入 decimal 才能使用下面的属性
    # getcontext().rounding = getattr(decimal, 'ROUND_HALF_EVEN')
    
    # 设置一个陷阱，当 Decimal 和 float 混合运算时会抛出异常。
    # 这是一个好习惯，可以防止不经意间的精度损失。
    getcontext().traps[FloatOperation] = True

    # Decimal 的构造方式之一：(符号, (数字元组), 指数)
    # (0, (1, 4, 1, 4), -3) 表示 +1.414
    d = Decimal((0, (1, 4, 1, 4), -3))
    print(d)

    # 进行高精度计算，这里的数字远超标准 int 或 double 的范围。
    a = Decimal(1 << 63) / Decimal(10**18)
    
    # 使用 f-string 格式化输出，可以指定小数位数。
    print(f"{a:.9f}") 

    # Decimal 对象支持常见的数学运算，并且保持精度。
    print(a.sqrt(), a.ln(), a.log10(), a.exp(), a ** 2)

def ComplexOperation():
    """
    复数运算。Python 原生支持复数。
    """
    # Python 使用 'j' 来表示虚数单位。
    a = 1 - 2j

    # a.real: 获取实部 (float)
    # a.imag: 获取虚部 (float)
    # a.conjugate(): 获取共轭复数
    print(a.real, a.imag, a.conjugate())

def FastIO():
    """
    在 Python 中实现快速 I/O 的一个常用模板。
    原理类似于 C++ 的 `ios::sync_with_stdio(false); cin.tie(NULL);`
    并且一次性读入所有数据，再一次性写出。
    """
    import sys, atexit, io

    # 1. 在程序开始时，一次性读取所有标准输入到内存中。
    _INPUT_LINES = sys.stdin.read().splitlines()
    
    # 2. 创建一个输入行的迭代器，并重载 `input()` 函数。
    # 之后每次调用 `input()` 都只是从内存中取下一行，而不是进行 I/O 操作。
    input = iter(_INPUT_LINES).__next__

    # 3. 创建一个内存中的输出缓冲区 (类似于 C++ 的 stringstream)。
    _OUTPUT_BUFFER = io.StringIO()
    
    # 4. 将标准输出重定向到这个内存缓冲区。
    # 之后所有的 `print()` 都会写入到这个缓冲区，而不是直接输出到屏幕。
    sys.stdout = _OUTPUT_BUFFER

    # 5. 注册一个程序退出时执行的函数。
    @atexit.register
    def write():
        # 在程序结束时，将缓冲区中的所有内容一次性写入到真正的标准输出。
        # 这种 "buffered write" 能极大地减少 I/O 次数，从而加速程序。
        sys.__stdout__.write(_OUTPUT_BUFFER.getvalue())

# --- 主程序逻辑在这里开始 ---
# 如果使用了 FastIO，建议将所有代码都放在一个函数内调用，避免全局作用域问题。
def main():
    # FastIO() # 如果需要，取消这行的注释
    # 示例：
    # line = input()
    # print(f"You entered: {line}")
    pass

if __name__ == '__main__':
    main()
