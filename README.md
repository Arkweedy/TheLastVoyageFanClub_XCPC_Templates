# XCPC Templates

这是华中科技大学 *哈斯特坐牢领域大神/终帆粉丝团* 队伍维护的 XCPC 线下赛纸质模板库。它的目标不是做一个“越全越好”的代码仓库，而是整理一份能在现场快速翻阅、页码可靠、索引好用、打印友好的比赛资料。

封面是 <font color="#00BFFF">Aqours 渡边曜</font>

如果你只是想使用这份模板，优先从 GitHub Releases 下载已经编译好的 `main.pdf`；如果你想修改内容、替换封面或重新排版，再阅读后面的编译与维护说明。

## 怎么使用

推荐使用方式：

1. 从 Releases 下载最新的 `main.pdf`。
2. 按比赛规则打印并装订。
3. 赛前熟悉目录和最后的索引页。
4. 比赛时优先按索引找关键词，再用页眉页码定位正文。

正文页码从 `Geometry` 开始计为第 1 页；封面、签名页和目录不参与正文页码。这是为了让目录、索引和打印页眉中的页码一致，减少现场翻页时的误差。

## 内容范围

模板按大类组织，当前主要包括：

- `Geometry`：二维/三维几何、凸包、半平面交、圆与球面等。
- `Tree and Graph`：图论、网络流、匹配、生成树、树上问题等。
- `Data Structure`：并查集、线段树、树状数组、Splay、LCT、动态树等。
- `String`：KMP、AC 自动机、SAM、PAM、Hash、Lyndon 等。
- `Math`：数论、线代、多项式、卷积、递推、单纯形等。
- `Tricks`：常用技巧、模型转换、矩阵树、日期自动机等。
- `Appendix` / `Miscellany`：公式表、常数表、运行脚本、对拍、初始模板等。

部分条目包含公式说明和代码两部分；例如 `min_25` 这类内容会把分析文字放在独立 `.tex` 文件中，代码仍然直接引用外部源码文件。

## 仓库结构

```text
main.tex                 文档入口，只负责总装配
tex/preamble.tex         页面样式、字体、索引、minted 和模板宏
tex/sections/            各大章节的条目清单
src/                     算法模板源码与专题说明
cover/src/               封面和签名页源码
cover/assets/            封面使用的图片素材
scripts/build.ps1        Windows PowerShell 构建脚本
scripts/rainbow_brackets.py
                         少量代码的彩虹括号试点生成器
```

## 修改模板

新增或修改算法模板时，优先保持“源码在 `src/`，章节清单在 `tex/sections/`”的结构。

常见入口：

```tex
\TemplateSection{大章节标题}
\TemplateSubsection{条目标题}
\TemplateCode{cpp}{src/Path/Template.cpp}
\TemplateNote{src/Path/Explanation.tex}
```

原则：

- 纯代码模板直接放在 `src/`，再用 `\TemplateCode` 引入。
- 需要公式或推导的专题，把说明写成独立 `.tex`，再和代码相邻排版。
- 不要在 `main.tex` 里堆具体条目；`main.tex` 只保留文档入口。
- 标题复杂、含中文或数学符号时，优先沿用现有 `\TemplateSubsection[sort-key]{title}` 写法，保证索引排序稳定。
- 封面成员、头像、版本号等信息在 `cover/src/cover.tex` 中维护，图片放在 `cover/assets/`。

## 编译

Windows 下推荐使用仓库自带脚本，在仓库根目录运行：

```powershell
.\scripts\build.ps1
```

构建完成后，根目录会生成：

```text
main.pdf
```

脚本会自动完成这些步骤：

- 编译两遍 `cover/src/cover.tex`，生成 `cover/dist/cover.pdf`。
- 编译两遍 `cover/src/signature.tex`，生成 `cover/dist/signature.pdf`。
- 生成彩虹括号试点文件。
- 编译两遍 `main.tex`。
- 默认清理 LaTeX 辅助文件和 minted 缓存。

如果需要保留辅助文件用于排查问题：

```powershell
.\scripts\build.ps1 -KeepAux
```

### 编译依赖

- TeX Live，至少需要 `xelatex`、`makeindex`。
- Python 和 Pygments，命令行中需要能找到 `python` 与 `pygmentize`。
- 本机字体需要包含文档中使用的字体，例如 `Linux Libertine O`、`Microsoft YaHei`、`SimHei`、`KaiTi`、`Consolas`。

`main.tex` 使用 `imakeidx` 生成索引；中文条目通过显式 sort key 保持拼音风格排序，不依赖额外的 xindy 中文模块。

### 手动编译

一般不需要手动敲完整命令；脚本是日常维护入口。确实需要手动编译时，可以参考：

```powershell
$env:PYTHONIOENCODING = "utf-8"
$env:PYTHONUTF8 = "1"

New-Item -ItemType Directory -Force -Path "cover/dist"
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -shell-escape -output-directory=cover/dist cover/src/cover.tex
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -shell-escape -output-directory=cover/dist cover/src/cover.tex
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -output-directory=cover/dist cover/src/signature.tex
xelatex -interaction=nonstopmode -halt-on-error -file-line-error -output-directory=cover/dist cover/src/signature.tex
python scripts/rainbow_brackets.py
xelatex -shell-escape -synctex=1 -interaction=nonstopmode -halt-on-error -file-line-error main.tex
xelatex -shell-escape -synctex=1 -interaction=nonstopmode -halt-on-error -file-line-error main.tex
```

## 发布 PDF

建议不要把 `main.pdf` 提交进 git。正式给队员或其他选手使用时，推荐通过 GitHub Release 发布。下面的 `$version` 替换为实际版本号，例如 `v2.0`、`v2.1`：

```powershell
$version = "v2.1"
git tag $version
git push origin main
git push origin $version
gh release create $version main.pdf --title "XCPC Templates $version" --notes "Printable XCPC/ICPC template PDF."
```

也可以在 GitHub 网页的 Releases 页面手动创建 release，然后上传根目录生成的 `main.pdf`。
