# XCPC Templates Agent 指南

本文件适用于整个仓库。它是给自动化 agent 的仓库入口，不替代面向使用者的 `README.md`。

## 1. 项目目标与不可破坏的约束

本仓库产出的是 ICPC/XCPC 线下赛使用的 A4 双面打印资料。所有修改首先服务于现场查找和抄写，而不是追求通用代码库式的抽象或网页阅读体验。

- 可检索性优先：目录、关键词索引、正文页眉页码必须互相一致。
- 打印可读性优先：避免代码越界、字号过小、低对比度配色和不必要的空白页。
- 维护成本要低：纯代码模板继续保存在外部源码文件中，通过 `minted` 引入。
- `main.tex` 只负责文档总装配，不在其中加入具体算法条目或大段公式。
- 不要因为文件当前未被引用就自行删除；归档和删除必须有明确任务依据。

## 2. 渐进式读取顺序

只读取完成当前任务所需的层级，不要默认扫描全部算法源码。

1. 先读本文件和 `README.md`，再运行 `git status --short`，确认任务边界与工作区状态。
2. 根据下方“任务导航”找到唯一相关的章节清单、源码目录或配置文件。
3. 修改算法条目前，只读该条目、相邻条目和它直接引用的文件；用 `rg` 搜索重名算法、关键函数和已有变体。
4. 只有在任务涉及全局排版、索引、封面或构建时，才继续读取 `tex/config/`、`cover/` 或 `scripts/`。
5. 只有需要比较旧实现或恢复历史材料时才进入 `src/archive/`；归档内容不属于当前 PDF。

## 3. 按任务导航

| 任务 | 首先读取 | 继续读取 |
| --- | --- | --- |
| 新增或替换算法代码 | 对应的 `tex/sections/*.tex` | 同类 `src/<Category>/` 文件及相邻条目 |
| 接入用户提供的代码片段 | 对应章节清单，使用 `rg` 查重 | 目标源码文件、同类模板接口和依赖 |
| 增加公式、推导或使用说明 | 相应章节清单 | 同目录 `.tex` 说明；参考 `src/Math/min25_notes.tex` |
| 调整章节、目录或关键词索引 | `tex/config/template-macros.tex` | 对应 `tex/sections/*.tex`、`tex/config/packages.tex` |
| 调整页码、页眉或双面打印布局 | `tex/config/frontmatter.tex` | `tex/config/layout.tex`、`main.tex` |
| 调整代码块样式 | `tex/config/minted.tex` | `tex/config/template-macros.tex` |
| 调整 C++ 彩虹括号 | `tex/config/rainbow-brackets.tex` | `scripts/rainbow_brackets.py`、`scripts/build.ps1` |
| 修改封面、成员或签名页 | `cover/src/cover.tex`、`cover/src/signature.tex` | `cover/assets/`、`tex/config/frontmatter.tex` |
| 排查编译或生成物 | `scripts/build.ps1` | `.gitignore`、相关 LaTeX 配置 |
| 延续已有维护计划 | `TODO.md` | 对应章节清单和源码；先确认条目仍符合当前意图 |
| 发布 PDF | `README.md` 的“发布 PDF” | GitHub Release，不修改构建布局 |
| 查看旧版或未采用模板 | `src/archive/` | Git 历史；不要直接重新接入正文 |

## 4. 正文章节索引

正文装配顺序由 `main.tex` 固定。新增条目通常只修改一个章节清单和对应源码目录。

| PDF 大章节 | 章节清单 | 主要源码目录 |
| --- | --- | --- |
| Geometry | `tex/sections/geometry.tex` | `src/Geometry/` |
| Tree and Graph | `tex/sections/tree_graph.tex` | `src/TreeandGraph/` |
| Data Structure | `tex/sections/data_structure.tex` | `src/DataStructure/` |
| String | `tex/sections/string.tex` | `src/String/` |
| Math | `tex/sections/math.tex` | `src/Math/` |
| Tricks | `tex/sections/tricks.tex` | `src/tricks/` |
| Appendix | `tex/sections/appendix.tex` | `src/TipsandHints/`，也会引用 `src/Math/` 和 `src/Miscellany/` |
| Miscellany | `tex/sections/miscellany.tex` | `src/Miscellany/` 和 `src/TipsandHints/` |

注意大小写：`src/tricks/` 与 `src/TipsandHints/` 用途不同。少量历史条目会跨目录引用，应以现有章节清单为准，不要为了目录整齐顺手搬迁。

## 5. 文档装配与配置索引

- `main.tex`：文档入口，仅加载 preamble、front matter、八个章节和索引。
- `tex/preamble.tex`：配置聚合入口，只按顺序加载 `tex/config/*.tex`。
- `tex/config/packages.tex`：包加载和 `imakeidx` 初始化。
- `tex/config/fonts.tex`：XeLaTeX 中英文字体与中文断行。
- `tex/config/layout.tex`：A4 双面页边距、装订偏移、页眉页码和标题样式。
- `tex/config/template-macros.tex`：章节、索引、说明和代码的公共宏。
- `tex/config/rainbow-brackets.tex`：C++ 彩虹括号开关与打印配色。
- `tex/config/frontmatter.tex`：封面、两张签名页、目录，以及正文从第 1 页开始的流程。
- `tex/config/minted.tex`：各语言代码块样式。
- `scripts/build.ps1`：唯一推荐的日常完整构建入口。
- `scripts/rainbow_brackets.py`：为 `src/**/*.cpp` 生成临时彩虹括号版本。

不要直接覆写 `\section`、`\subsection` 或全局 `minted` 行为；需要全局行为时修改对应配置文件，并验证所有受影响章节。

## 6. 章节宏约定

章节清单中优先使用以下接口：

```tex
\TemplateSection[sort-key]{大章节标题}
\TemplateSubsection[sort-key]{可检索的模板条目}
\TemplateSubsubsection{条目内部的变体}
\TemplateNote{src/Category/Explanation.tex}
\TemplateCode{cpp}{src/Category/Template.cpp}
\TemplateCode[highlightlines={7,12}]{cpp}{src/Category/Template.cpp}
```

- `\TemplateSection` 和 `\TemplateSubsection` 会同时加入关键词索引。
- `\TemplateSubsubsection` 会进入目录，但默认不加入关键词索引，适合一个算法条目下的多个实现。
- `*Star` 版本不编号，也不会自动加入索引，只用于确实不应成为查找入口的短说明。
- `\TemplateNote` 引入正文片段；对应 `.tex` 不应再声明 `\documentclass` 或完整文档环境。
- `\TemplateCode` 的第一个可选参数原样传给 `minted`。
- C++ 条目默认使用构建脚本生成的彩虹括号文件；生成文件缺失或开关关闭时自动回退到原源码。
- 不要在章节清单中直接使用 `\inputminted`，除非公共宏确实无法表达需求并且任务明确要求扩展它。

中文或含数学符号的标题必须提供稳定的 `sort-key`。现有 `K...` 键属于当前历史排序体系，不要手工猜测、批量重写或混入另一套未经验证的键。仓库目前没有排序键生成脚本；新增中文索引键若无法可靠复现现有规则，应先向维护者说明，再决定补生成工具还是采用经完整索引验证的新策略。

## 7. 接入新模板的标准流程

收到现有文件或代码片段后，按以下顺序处理：

1. 明确条目标题、目标章节、语言，以及需要“纯代码”还是“说明 + 代码”；影响归类或排版的信息不足时先询问。
2. 使用 `rg` 搜索算法名、关键函数名和独特代码片段，判断是新增、替换还是并列变体。搜索正文时排除 `src/archive/`，需要历史比较时再单独搜索归档。
3. 阅读目标章节中前后相邻的条目，沿用其文件命名、标题、排版层级和相邻内容组织方式。
4. 将可直接抄写的代码放入对应 `src/<Category>/`。不要把代码粘进 `main.tex` 或章节清单。
5. 若需要公式或分析，新建同目录专题 `.tex`，并按“说明在前、代码紧随其后”使用 `\TemplateNote` 与 `\TemplateCode`。
6. 在对应 `tex/sections/*.tex` 的合适位置登记条目。除非用户要求，不重排无关条目。
7. 运行完整构建，检查目录、索引、页码、代码换行以及受影响页面的打印可读性。
8. 查看 `git diff` 和 `git status --short`，确认没有生成物、缓存、无关格式化或用户已有改动混入。

本仓库 agent 的职责是将用户提供的材料保真录入、分类并排版，不负责评审、证明、测试或修复算法正确性。除路径、编码和 LaTeX 接入所需的最小处理外，不要改写源码内容；发现看起来可疑的实现也不要在录入任务中主动展开正确性检查。

## 8. 构建与排版验证

日常完整构建：

```powershell
.\scripts\build.ps1
```

需要保留辅助文件排查索引或 LaTeX 问题时：

```powershell
.\scripts\build.ps1 -KeepAux
```

构建脚本会编译封面和签名页各两遍、生成全部 C++ 彩虹括号文件，再编译主文档两遍。不要再手工追加额外编译轮次。默认构建结束会清理辅助文件和 `build/`；`-KeepAux` 产生的调试文件在任务结束前应恢复到合理状态。

以下检查只确认材料被正确引用并呈现在 PDF 中，不包含算法正确性验证：

- 仅改 Markdown：检查链接、命令、路径和 `git diff`，通常不必重建 PDF。
- 仅录入但尚未接入正文的源码：检查目标路径、文件编码和 `git diff`，不要求语法、样例或 checker 测试。
- 改源码引用、章节清单、说明 `.tex` 或任意排版配置：运行 `.\scripts\build.ps1`。
- 改页码、目录、索引、封面、代码块样式或彩虹括号：完整构建后检查相关 PDF 页面；不能只以命令退出码作为排版验收。
- 正文第一页仍应是 `Geometry` 的 Page 1；封面、两张签名页和目录不参与正文页码。

## 9. 文件卫生与 Git 边界

- 不提交根目录 `main.pdf`、`cover/dist/`、`build/`、`_minted*`、LaTeX 辅助文件或本地 IDE 配置。
- `src/archive/` 保存未采用、被替换或仅供参考的材料；默认不编辑、不接入正文。彩虹脚本目前会递归扫描整个 `src/`，所以使用 `-KeepAux` 时可能出现 `build/rainbow/src/archive/` 临时副本，这不表示归档内容进入了 PDF。
- 不新建按贡献者命名的活动源码目录；新模板按知识点归类。需要保留来源时在代码注释或提交说明中记录。
- 不使用绝对本机路径，不引入比赛现场或构建机器必须联网才能获得的资源。
- 开始前保留用户已有工作区改动；不要回退、覆盖或顺手提交无关内容。
- 除非用户明确要求先确认或不提交，完成修改和必要的录入、排版检查后，默认创建一次本地 commit。
- 不默认 push。只有用户明确要求时才执行 push、tag 或 GitHub Release。
- 修改应按单一目的组织；避免算法内容、全局排版和仓库清理混在同一提交中。

## 10. 完成说明

最终回复至少说明：修改了哪些条目或配置、为何这样归类、执行了哪些录入与排版检查、PDF 是否重建，以及是否已创建本地 commit。若构建失败，要区分路径或编码、LaTeX/Pygments/字体依赖和本机环境问题，不要把失败产物描述为可发布版本。
