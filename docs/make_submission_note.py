from docx import Document
from docx.shared import Pt, RGBColor, Inches
from docx.enum.text import WD_ALIGN_PARAGRAPH


def add_heading(doc, text, size=14):
    p = doc.add_paragraph()
    run = p.add_run(text)
    run.bold = True
    run.font.size = Pt(size)
    run.font.color.rgb = RGBColor(0x1A, 0x53, 0x8B)
    p.paragraph_format.space_before = Pt(12)
    p.paragraph_format.space_after = Pt(4)
    return p


def add_body(doc, text):
    p = doc.add_paragraph()
    run = p.add_run(text)
    run.font.size = Pt(11)
    p.paragraph_format.space_after = Pt(4)
    return p


def add_bullet(doc, text):
    p = doc.add_paragraph(style='List Bullet')
    p.clear()
    run = p.add_run(text)
    run.font.size = Pt(11)
    p.paragraph_format.left_indent = Inches(0.3)
    p.paragraph_format.space_after = Pt(2)
    return p


def add_separator(doc):
    p = doc.add_paragraph()
    run = p.add_run('─' * 72)
    run.font.size = Pt(8)
    run.font.color.rgb = RGBColor(0xCC, 0xCC, 0xCC)
    p.paragraph_format.space_before = Pt(4)
    p.paragraph_format.space_after = Pt(4)
    return p


doc = Document()
for section in doc.sections:
    section.top_margin = Inches(1)
    section.bottom_margin = Inches(1)
    section.left_margin = Inches(1.2)
    section.right_margin = Inches(1.2)

# Title
title = doc.add_paragraph()
title.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = title.add_run('MindTrapa')
r.font.size = Pt(24)
r.bold = True
r.font.color.rgb = RGBColor(0x1A, 0x53, 0x8B)

sub = doc.add_paragraph()
sub.alignment = WD_ALIGN_PARAGRAPH.CENTER
r2 = sub.add_run('提出課題 概要 / Submission Overview — イヴァン ダニシェウスキ (Ivan Danisheuski)')
r2.font.size = Pt(11)
r2.font.color.rgb = RGBColor(0x88, 0x88, 0x88)
sub.paragraph_format.space_after = Pt(10)

add_separator(doc)

# ══════════════════════ JAPANESE ══════════════════════

add_heading(doc, '制作期間', 14)
add_body(doc,
    '2025年6月〜2026年7月（約13ヶ月）。学業と並行したソロ開発のため、学期の区切りで '
    '中断を挟みながら、実質的な作業期間は5つのフェーズ合計で約4ヶ月です。')
add_bullet(doc, '2025年6月　　操作・カメラのプロトタイプ')
add_bullet(doc, '2025年10〜11月　Block 1〜2（レベル1〜20）完成')
add_bullet(doc, '2026年1月　　Block 3（レベル21〜30）完成')
add_bullet(doc, '2026年4〜5月　Block 4、3Dモデルパイプライン、プレイヤーアニメーション、プロファイル/セーブシステム')
add_bullet(doc, '2026年6〜7月　Block 5完成、リファクタリング、多言語対応（EN/RU/JA）、最終調整')

add_heading(doc, 'アピールポイント', 14)
add_bullet(doc, '全50レベル・17種のユニークメカニクスを完成させ、コンテンツを最後まで作り切った実績')
add_bullet(doc, '既製エンジンなしで物理エンジン（AABB衝突判定、重力ゾーン・逆重力含む）を独自実装')
add_bullet(doc, 'プレイヤーキャラクターの完全なアニメーションステートマシン、カメラシステムを独自構築')
add_bullet(doc, 'コード品質へのこだわり：終盤に第三者視点での独立コードレビューを自ら実施し、'
                '36件の指摘（Critical含む）を全て修正・検証してからバージョン1.0として確定')
add_bullet(doc, '3言語ローカライズ（EN/RU/JA、キリル文字・かな用フォント対応）、'
                'セーブ/プロファイルシステムまで一人で設計・実装')
add_bullet(doc, 'レベルデザイン・物理・カメラ・UI・メニュー・オーディオ・ローカライズ、'
                'すべての領域を単独で担当')

add_heading(doc, '使用技術・アセット', 14)
add_bullet(doc, '言語・グラフィック：C++、DxLib 3.24f（DirectXラッパー、商用エンジン不使用）')
add_bullet(doc, '3Dモデル：Quaternius / Kenney / KayKit（すべてCC0ライセンス）。OBJ→DirectX .x '
                '変換用の自作ツールを作成し、パイプラインを構築')
add_bullet(doc, '音楽：Suno AIで生成（ブロックごとに1曲、全5曲）')
add_bullet(doc, '効果音：Freesound.org（CC0ライセンス）')
add_bullet(doc, 'フォント：Orbitron（UI、SIL OFL 1.1）、Exo 2 / Noto Sans JP（ロシア語・日本語ローカライズ用）')

add_heading(doc, '開発体制', 14)
add_body(doc,
    '本作はチーム開発ではなく、企画・レベルデザイン・プログラミング・3Dパイプライン構築・'
    'ローカライズ・音響実装まで、すべて私一人で行ったソロ開発作品です。')

add_separator(doc)

# ══════════════════════ ENGLISH ══════════════════════

add_heading(doc, 'Production Period', 14)
add_body(doc,
    'June 2025 - July 2026 (~13 months). Solo development alongside full-time study, '
    'with breaks between academic terms — actual active work totals roughly 4 months '
    'across 5 distinct phases.')
add_bullet(doc, 'Jun 2025 — control/camera prototype')
add_bullet(doc, 'Oct-Nov 2025 — Block 1-2 complete (levels 1-20)')
add_bullet(doc, 'Jan 2026 — Block 3 complete (levels 21-30)')
add_bullet(doc, 'Apr-May 2026 — Block 4, 3D model pipeline, player animation, profile/save system')
add_bullet(doc, 'Jun-Jul 2026 — Block 5 complete, refactor, localization (EN/RU/JA), final polish')

add_heading(doc, 'Key Strengths', 14)
add_bullet(doc, 'All 50 levels and 17 distinct mechanics shipped complete — followed through to a finished product, not a partial prototype')
add_bullet(doc, 'Custom physics engine with no commercial game engine: AABB collision, gravity zones including reversed gravity, ice inertia')
add_bullet(doc, 'Built the player animation state machine and camera system from scratch')
add_bullet(doc, 'Deliberate focus on code quality: ran an independent multi-pass code review on the finished game near the end of development, found and fixed 36 issues (including critical collision/physics bugs) before calling it version 1.0')
add_bullet(doc, 'Designed and implemented the full localization system (EN/RU/JA, separate font handling for Cyrillic/Kana) and the save/profile system solo')
add_bullet(doc, 'Sole owner of every system: level design, physics, camera, UI, menus, audio, localization')

add_heading(doc, 'Technologies & Assets Used', 14)
add_bullet(doc, 'Language / graphics: C++, DxLib 3.24f (DirectX wrapper, no commercial engine)')
add_bullet(doc, '3D models: Quaternius / Kenney / KayKit (all CC0) — wrote a custom OBJ-to-DirectX-.x converter tool to build the asset pipeline')
add_bullet(doc, 'Music: generated with Suno AI (one track per block, 5 total)')
add_bullet(doc, 'SFX: Freesound.org (CC0)')
add_bullet(doc, 'Fonts: Orbitron (UI, SIL OFL 1.1), Exo 2 / Noto Sans JP (Russian/Japanese localization)')

add_heading(doc, 'Development Structure', 14)
add_body(doc,
    'Not a team project — design, level design, programming, 3D pipeline, localization, '
    'and audio implementation were all done by me alone.')

add_separator(doc)

footer = doc.add_paragraph()
footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
r = footer.add_run('van4ous00@gmail.com  ·  github.com/BarriRin/MindTrapa')
r.font.size = Pt(9)
r.font.color.rgb = RGBColor(0x99, 0x99, 0x99)

out_path = r'D:\Probuem\MindTrapa\docs\MindTrapa_Submission_Overview.docx'
doc.save(out_path)
print(f'Saved: {out_path}')
