/****************************************************************************
** Form implementation generated from reading ui file 'ui\geodma_window.ui'
**
** Created: seg 7. nov 09:31:02 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "geodma_window.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qtoolbutton.h>
#include <qlistbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtoolbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../ui/geodma_window.ui.h"
static const unsigned char const image0_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x19,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xe9, 0x85, 0x63, 0x00, 0x00, 0x00,
    0x74, 0x49, 0x44, 0x41, 0x54, 0x48, 0x89, 0xed, 0x94, 0x5d, 0x0a, 0x00,
    0x21, 0x08, 0x84, 0x6d, 0xd9, 0x53, 0x79, 0x7f, 0xe6, 0x5a, 0xed, 0x93,
    0xb0, 0x04, 0xfe, 0x45, 0xb1, 0x4b, 0x38, 0x2f, 0x06, 0x89, 0x9f, 0x43,
    0x5a, 0x03, 0x40, 0xbb, 0x75, 0x6d, 0x27, 0x1c, 0x05, 0xb9, 0x33, 0xc9,
    0xcc, 0xdc, 0xe5, 0x0c, 0xa0, 0x2d, 0x81, 0xbc, 0x8b, 0x8e, 0x85, 0xad,
    0xbb, 0x51, 0x4d, 0x9b, 0x2e, 0x66, 0xee, 0x99, 0x6e, 0xad, 0xfc, 0x94,
    0x93, 0x59, 0xa9, 0x4e, 0x04, 0x12, 0x71, 0xe3, 0xe5, 0x85, 0xa6, 0x4b,
    0x1c, 0x69, 0xd1, 0xd3, 0x39, 0x7b, 0x52, 0x90, 0x82, 0x7c, 0x0c, 0x91,
    0x6d, 0xd6, 0xa2, 0x27, 0xf3, 0x5b, 0x21, 0x8a, 0x6d, 0xb5, 0x07, 0x73,
    0x21, 0x2b, 0xf4, 0x9f, 0x37, 0x29, 0x88, 0xe8, 0x01, 0x37, 0xbf, 0x3d,
    0x7b, 0x98, 0x16, 0x69, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e,
    0x44, 0xae, 0x42, 0x60, 0x82
};

static const unsigned char const image1_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x14,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x8d, 0x89, 0x1d, 0x0d, 0x00, 0x00, 0x00,
    0xb9, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xad, 0x95, 0xc1, 0x11, 0xc3,
    0x20, 0x0c, 0x04, 0x97, 0x8c, 0x4b, 0xd2, 0x8b, 0x1e, 0x5c, 0x07, 0x35,
    0xb9, 0x0e, 0xf5, 0xc0, 0xcb, 0x3d, 0x91, 0x4f, 0x84, 0x1d, 0x82, 0x83,
    0x98, 0xe1, 0x5e, 0x30, 0x86, 0xd5, 0x9d, 0x05, 0x43, 0xc8, 0x39, 0xb3,
    0x52, 0x1b, 0x40, 0x8c, 0xb1, 0xac, 0x80, 0xe5, 0x9c, 0xc3, 0x66, 0x93,
    0x7d, 0xdf, 0x87, 0x1b, 0x54, 0x95, 0xe3, 0x38, 0xba, 0xdf, 0x52, 0x4a,
    0x00, 0xbc, 0xbc, 0xd5, 0x55, 0xd5, 0xb5, 0xce, 0x0d, 0x04, 0x1e, 0xdd,
    0x4d, 0x03, 0xbd, 0xee, 0xe0, 0xd3, 0x94, 0x11, 0xcc, 0xe3, 0xcc, 0xf4,
    0xe8, 0x50, 0x55, 0xab, 0x33, 0xfb, 0xe1, 0x1e, 0x7d, 0x39, 0xbc, 0x47,
    0x3b, 0xcf, 0xb3, 0x8e, 0x45, 0x84, 0x94, 0x92, 0xcb, 0x69, 0x05, 0x1a,
    0xec, 0x0e, 0x6a, 0xe1, 0x22, 0x32, 0x84, 0x56, 0x60, 0x0f, 0xd4, 0x53,
    0x1b, 0xbf, 0x2d, 0x30, 0x6c, 0x8a, 0x49, 0x44, 0x00, 0x28, 0xe5, 0xba,
    0x54, 0x21, 0x84, 0x9f, 0x02, 0x2e, 0x60, 0x0f, 0xd6, 0x83, 0xc3, 0xc4,
    0xc1, 0x6e, 0x61, 0x4f, 0x9a, 0xba, 0x29, 0x4b, 0x80, 0x16, 0x77, 0x19,
    0x10, 0xfc, 0x71, 0xdd, 0xc0, 0x19, 0xd5, 0x2e, 0xff, 0x8b, 0x66, 0x1d,
    0xf4, 0x28, 0xac, 0x7e, 0x02, 0xde, 0x3c, 0xc0, 0x47, 0x89, 0x7b, 0xd3,
    0x5d, 0x66, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42,
    0x60, 0x82
};

static const char* const image2_data[] = { 
"35 30 533 2",
"fT c #040406",
"fR c #040507",
"eW c #08090b",
"dG c #09090c",
"e1 c #0a0b0f",
"fW c #0b0d0f",
"ft c #0c0e10",
"fo c #0d0e10",
"eK c #0f1112",
"el c #101111",
"ew c #121214",
"eh c #131416",
"ez c #161719",
"eQ c #161817",
"d2 c #171717",
"fP c #18181a",
"d8 c #1a1a1a",
"eE c #1a1b1d",
"e# c #1a1b1e",
"eH c #1b1c1d",
"f# c #1c1d1f",
"fM c #1c1e1d",
"fq c #1d1d1e",
"et c #1d1d1f",
"eZ c #1d1d20",
"fZ c #1f2022",
"ei c #202121",
"em c #222423",
"fi c #222524",
"fC c #232426",
"eq c #242525",
"eB c #252627",
"ee c #252628",
"en c #262727",
"fH c #27282a",
"dH c #28292b",
"f1 c #2a2b2d",
"eN c #2a2c2e",
"du c #2e2e30",
"ds c #2e2f30",
"e4 c #2f3031",
"fe c #303133",
"fm c #343537",
"dt c #353638",
"eM c #353639",
"fU c #38383b",
"eg c #38393b",
"fn c #393a3c",
"dF c #3a3c3d",
"fI c #3b3d3f",
"ep c #3f4041",
"fl c #3f4042",
"fa c #404143",
"gk c #434345",
"ff c #434446",
"ec c #444547",
"dx c #454546",
"e6 c #454647",
"fx c #494a4c",
"gp c #494b4b",
"dv c #4a4a4b",
"ej c #4b4c4c",
"e7 c #4b4d4e",
"fy c #4c4e4f",
"d7 c #4f4f4f",
"fY c #505052",
"gh c #505152",
"dD c #535353",
"f7 c #535454",
"gq c #535455",
"eV c #535456",
"dA c #545454",
"fv c #545456",
"e3 c #545556",
"gl c #565657",
"gf c #575758",
"ey c #575759",
"f4 c #58585a",
"f8 c #58595b",
"eI c #595a5d",
"d5 c #5a5a5b",
"ek c #5a5b5b",
"fD c #5a5b5d",
"ed c #5b5b5d",
"fQ c #5c5d5e",
"gd c #5d5e5f",
"gg c #5d5e60",
"fV c #5f5f61",
"gr c #606161",
"eA c #616262",
"f2 c #616263",
"d1 c #626262",
"eP c #656766",
"fL c #666766",
"dI c #676869",
"g# c #68696a",
"fh c #696b6a",
"es c #6c6c6d",
"fS c #6d6c6e",
"eY c #6e6e6f",
"fb c #6e6f71",
"fc c #6e7071",
"dC c #717171",
"ga c #747576",
"f9 c #757677",
"eJ c #757678",
"eC c #7a7b7b",
"d9 c #7e7e7e",
"eL c #7f8182",
"dw c #808080",
"go c #858586",
"fX c #88898b",
"dL c #898989",
"e2 c #8b8b8d",
"fu c #8b8c8e",
"ea c #8d8e8f",
"ex c #8f8e90",
"dY c #8f8f8f",
"fA c #8f9090",
"gm c #909193",
"dK c #919192",
"gc c #929292",
"dr c #929393",
"e9 c #929494",
"f0 c #949597",
"eF c #949696",
"dV c #979898",
"dX c #989898",
"gj c #989899",
"f6 c #9e9e9e",
"dT c #9e9f9e",
"eo c #9e9fa0",
"fs c #9fa0a1",
"dE c #a1a2a3",
"dO c #a2a3a3",
"eU c #a2a3a5",
"ev c #a3a4a4",
"eR c #a3a5a4",
"dP c #a4a5a5",
"ef c #a4a5a6",
"dR c #a6a6a6",
"eX c #a8a8a9",
"fd c #a8a9aa",
"dQ c #aaaaaa",
"fp c #aaaaab",
"gn c #ababac",
"dB c #acacac",
"f5 c #acadad",
"d6 c #adadae",
"dU c #b1b2b1",
"dS c #b3b3b3",
"dJ c #b6b6b6",
"eO c #b7b8b8",
"dM c #b8b8b9",
"dz c #bbbbbb",
"eT c #bdbec0",
"d4 c #bebebf",
"fE c #bebfc0",
"fO c #c0c0c0",
"gi c #c0c1c2",
"dZ c #c2c3c3",
"e. c #c2c3c4",
"dW c #c3c4c4",
"fz c #c5c5c6",
"fr c #c5c5c7",
"er c #c7c7c7",
"e8 c #c7c7c8",
"d3 c #c8c8c8",
"eD c #c9c9ca",
"eu c #cacacb",
"fF c #cacccd",
"e0 c #cccccd",
"gs c #cdcece",
"eS c #cecece",
"f. c #cecfcf",
"e5 c #d0d1d3",
"eG c #d1d2d2",
"dy c #d3d3d3",
"f3 c #d6d6d7",
"g. c #d7d8d8",
"fw c #d7d8d9",
"fG c #d9dbdc",
"fB c #dadbdb",
"fN c #e0e1e0",
"eb c #e1e2e2",
"dm c #e3e3e3",
"fk c #e3e4e4",
"fj c #e6e7e7",
"cz c #e7a849",
"cC c #e7a94b",
"#n c #e7bd72",
"de c #e7e7e7",
"df c #e7e7e8",
"ax c #e8a33b",
"cg c #e8a746",
"#L c #e8a748",
"#X c #e8a847",
"aQ c #e8ab4b",
"bk c #e8ab4d",
".L c #e8af55",
"bv c #e8af57",
"bs c #e8af58",
"#N c #e8b35c",
"am c #e8b661",
"a1 c #e9a33d",
"#H c #e9a43f",
"#t c #e9a540",
"bm c #e9a641",
"#. c #e9a642",
"aI c #e9a743",
"bl c #e9a744",
".T c #e9a745",
"cl c #e9a747",
".C c #e9a948",
".7 c #e9a949",
".q c #e9ad51",
"cY c #e9ad53",
"bT c #e9af52",
".W c #e9af55",
"#m c #e9b054",
"ca c #e9b158",
"ba c #e9b35a",
"cX c #e9b562",
"#U c #e9b660",
"aX c #e9b663",
".2 c #e9b863",
"bh c #e9b866",
"bi c #e9b868",
"cZ c #e9b969",
".u c #e9bb71",
"bO c #e9bc72",
"#V c #e9be6f",
"b9 c #e9be72",
"aF c #e9c175",
"dj c #e9e9e9",
".E c #eaa237",
".D c #eaa33a",
"be c #eaa33c",
"cP c #eaa33d",
"b5 c #eaa43d",
".3 c #eaa53e",
"#v c #eaa641",
"cr c #eaa644",
"cd c #eaa743",
"cc c #eaa744",
"aD c #eaa745",
"#5 c #eaa947",
"ag c #eaa94a",
"aR c #eaaa48",
".N c #eaaa4a",
"#W c #eaab4a",
"an c #eaab4b",
"#8 c #eaab4c",
"cH c #eaab4e",
"bI c #eaac4c",
"au c #eaac50",
".F c #eaad4e",
"bJ c #eaaf50",
"bB c #eaaf51",
".Y c #eaaf54",
"bc c #eaaf55",
".p c #eaaf57",
"bK c #eab054",
".1 c #eab056",
"aH c #eab35a",
".H c #eab45a",
"bV c #eab45c",
"bD c #eab45f",
"a4 c #eab461",
"al c #eab55f",
"a8 c #eab65f",
"#E c #eab660",
"c1 c #eab664",
"a5 c #eab665",
"#R c #eab763",
"a9 c #eab862",
"az c #eab968",
"#9 c #eab969",
"c0 c #eaba6d",
"#O c #eabc6e",
"#4 c #eabd6d",
"a7 c #eabd72",
"#F c #eabe71",
"aP c #eabf72",
"bQ c #eabf7b",
"bX c #eac076",
"aE c #eac176",
"bj c #eac179",
"dg c #eaeaea",
".M c #eba13b",
"cJ c #eba238",
"ac c #eba239",
"cO c #eba338",
"#1 c #eba339",
"cI c #eba33b",
"bn c #eba43b",
"aC c #eba43c",
"ay c #eba43d",
".J c #eba43e",
".Z c #eba53c",
"#C c #eba53d",
"bq c #eba53e",
"#Q c #eba540",
"aJ c #eba541",
"a0 c #eba63d",
"a. c #eba63e",
"#y c #eba641",
"#T c #eba642",
"#j c #eba73d",
"#b c #eba741",
"ab c #eba743",
"#f c #eba744",
"cb c #eba745",
".I c #eba944",
"cA c #eba945",
"br c #eba946",
"#G c #eba947",
"b4 c #ebaa48",
"bH c #ebaa49",
"#e c #ebaa4a",
"bC c #ebaa4b",
"bS c #ebab4b",
"ct c #ebac4b",
"ah c #ebac4f",
"aS c #ebad4c",
"a# c #ebad4f",
"aM c #ebae4e",
"#S c #ebae51",
"aq c #ebae54",
"cL c #ebaf52",
"aA c #ebb05b",
"#z c #ebb157",
"#3 c #ebb158",
"b3 c #ebb159",
"bU c #ebb35a",
"aO c #ebb35d",
"aU c #ebb55f",
"aa c #ebb560",
".r c #ebb561",
"b0 c #ebb65f",
"b# c #ebb661",
"bW c #ebb662",
"bp c #ebb762",
"#A c #ebb765",
"bE c #ebb766",
"bZ c #ebb863",
".K c #ebb865",
"ao c #ebb964",
"#d c #ebb967",
"#w c #ebba68",
".G c #ebba6a",
"#g c #ebbb6b",
"bw c #ebbb6e",
"ap c #ebbc6c",
"#0 c #ebbc72",
".X c #ebbd6f",
".B c #ebbd74",
"ai c #ebbf76",
"bx c #ebc176",
"cG c #ebc481",
"dp c #ebebeb",
".6 c #eca235",
"cM c #eca239",
"cN c #eca23a",
"cf c #eca337",
"cK c #eca339",
"aB c #eca33a",
"#M c #eca33d",
".0 c #eca439",
"at c #eca43a",
".U c #eca43b",
"#l c #eca43c",
"#6 c #eca43d",
"aZ c #eca43f",
".5 c #eca53a",
"## c #eca53b",
"#i c #eca53c",
".4 c #eca53d",
".V c #eca53e",
"#u c #eca53f",
"bd c #eca63d",
"#B c #eca63e",
"#k c #eca63f",
"#D c #eca640",
"#P c #eca641",
"bR c #eca741",
"aY c #eca845",
"bG c #eca946",
"aN c #ecac4c",
"#h c #ecac4d",
"bb c #ecae4f",
"aT c #ecaf51",
"bL c #ecb25a",
"b2 c #ecb55d",
"bz c #ecb560",
"#c c #ecb662",
"b1 c #ecb75f",
"ck c #ecb762",
"#x c #ecb765",
"bM c #ecb863",
"by c #ecb864",
".t c #ecb96b",
".o c #ecb96f",
"cu c #ecba69",
"aV c #ecba6b",
"aG c #ecbc6d",
"c2 c #ecc179",
"ad c #ecc17e",
"bY c #ecc37c",
"b6 c #ecc383",
"aW c #ecc989",
"#I c #ecc98c",
".v c #ecce96",
"bo c #eda338",
"cv c #eda33b",
"aj c #eda43c",
"c# c #eda43d",
"c. c #eda43e",
"ce c #eda53b",
"#a c #eda53d",
"ak c #eda53e",
"ar c #eda53f",
"#7 c #eda540",
"#2 c #eda63d",
"as c #eda63f",
"bF c #eda945",
"bA c #edb259",
"b. c #edb864",
"bN c #edb867",
"cB c #edbc6e",
".s c #edbd70",
".O c #edbe75",
"cW c #edc078",
"cQ c #edc382",
"a6 c #edc785",
".9 c #edcb91",
"cs c #eea33b",
"cy c #eece9a",
".S c #eed09e",
"cD c #eed1a0",
"#o c #eed2a0",
"a2 c #eed6a9",
"do c #eeeeee",
"dN c #eeefef",
".n c #efcb91",
"#s c #efcb95",
"cq c #efcf99",
".w c #efcf9b",
"bf c #efd7aa",
".8 c #efdcb4",
"di c #efefef",
"cV c #f0cf9b",
"cm c #f0cf9d",
"c3 c #f0d2a1",
"cw c #f0d4a3",
"cj c #f0d5a7",
"dn c #f1f1f1",
"aL c #f2d6aa",
"aw c #f2d8ad",
"a3 c #f2dab0",
"#K c #f2ddb6",
"b8 c #f2e3c6",
"cF c #f2e4c3",
"dd c #f2f2f2",
".A c #f3dcb6",
"aK c #f3debb",
"dl c #f3f3f3",
"fJ c #f3f4f5",
".P c #f4deba",
"bg c #f4e1c0",
"#Y c #f4e2c1",
"bt c #f4e2c3",
".m c #f4e4c6",
"cR c #f5e2c4",
"ch c #f5e3c4",
"af c #f5e7cb",
"bu c #f5e8ce",
"d. c #f6e5c7",
"c9 c #f6e6cb",
"da c #f6e6cc",
".x c #f6e7cb",
"cU c #f6e9d1",
"gb c #f6f7f7",
"d# c #f7e8ce",
"c4 c #f7ebd5",
"c8 c #f7f1e0",
"ge c #f7f7f7",
".g c #f8e4c5",
".h c #f8e7cd",
"av c #f8ebd6",
"db c #f8f2e3",
"dk c #f8f8f8",
".f c #f9e7cb",
".e c #f9f1df",
"bP c #f9f1e2",
".j c #f9f3e6",
".i c #faf1e2",
"#p c #faf8f0",
"dh c #fafafa",
"fg c #fafbfc",
"#Z c #fbf9f1",
".d c #fbf9f2",
".l c #fbf9f3",
".k c #fbf9f4",
"c7 c #fbfaf2",
"dc c #fbfaf4",
".y c #fbfaf5",
"dq c #fbfbfb",
"#r c #fcf8f1",
".z c #fcfaf4",
"cp c #fcfaf5",
"#J c #fcfbf4",
"ae c #fcfbf7",
"b7 c #fcfbf8",
"cE c #fcfcf7",
".Q c #fcfcf8",
"cS c #fcfcf9",
"d0 c #fcfcfc",
"fK c #fcfdfd",
".R c #fdfbf6",
"cn c #fdfbf8",
"cx c #fdfcf8",
"ci c #fdfdfa",
"cT c #fdfdfc",
"Qt c #fdfdfd",
".# c #fdfdfe",
"c6 c #fdfefe",
".a c #fdfeff",
".c c #fdfffe",
"co c #fefefe",
"#q c #fefeff",
".b c #feffff",
"c5 c #ffffff",
"QtQtQtQtQtQtQtQtQtQt.#.a.b.c.d.e.f.g.h.i.j.k.b.b.a.#QtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQt.a.b.l.m.n.o.p.q.r.s.t.u.v.w.x.y.b.aQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQt.b.z.A.B.C.D.E.F.G.H.I.J.K.L.M.N.O.P.Q.aQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt.b.R.S.T.E.U.V.W.X.Y.Z.0.1.2.3.4.5.6.7.8.c.bQtQtQtQtQtQt",
"QtQtQtQtQtQt.b.z.9#.###a#b#c#d#e.4#a#f#g#h#i#j#k#l#m#n#o#p.bQtQtQtQtQt",
"QtQtQtQtQt#q#r#s#t#l#u#v#w#x#y.4#k#i#z#A.4#B#C#D#E#F#G#H#I#J#qQtQtQtQt",
"QtQtQtQt.#.b#K#L#l.V#M#N#O#P#i#B#u#Q#R#S#l#l#T#U#V#W#l.U#X#Y.b.#QtQtQt",
"QtQtQtQt#q#Z#0#1#2#l#3#4#5#6#u#7.V#8#9#Da.a##waaab.4#k#7acadae#qQtQtQt",
"QtQtQtQt.bafag#i#lahaiahaj#u#uak.Zalamanaoapaq.3.Var#kasatauav.bQtQtQt",
"QtQtQt.#.bawax.UayazaAaB#i.4#laCaDaEaFaGaHaIay#karar#u#k.ZaJaK.bQtQtQt",
"QtQtQt.#.baLaMaNaOaPaQaRaSaT#zaUaVaWaXaYaZ.V#B#k#u#u#ua0#ia1a2.b.#QtQt",
"QtQtQt.#.ba3a4a5a6a7a8a9b.b#babbbc#9#v#i#k#u#u#u#u#ubdbdajbebf.b.#QtQt",
"QtQtQtQt.bbgbhbibjbkblbm#6bnbnbo.Fbp#Q#i.4.V.4.4#6#6bq#bbrbsbt.bQtQtQt",
"QtQtQtQt.bbubvbwbxbybybzbAbBahbCbDbEbrbFbGbHbIbJbKbLbMbN#cbObP.bQtQtQt",
"QtQtQtQt#q#rbQ#RaRbRbSbTbUbVbWaabXbYbpbybZa9b0b1b2b3#Sb4b5b6b7#qQtQtQt",
"QtQtQtQt.##qb8b9.4c.c##C.Z.V#Q.3ca#Rblcbcccd#y.V#i#lcecfcgch.b.#QtQtQt",
"QtQtQtQtQt.acicjbm.U#k#B#k#u#B#i#Sck#Q.V.V#B#k#Basar.5clcmcncoQtQtQtQt",
"QtQtQtQtQtQt.bcpcqcrcsak#k#u#u#Cctcucb.4#u#u.V.Vakcv#Xcwcx.aQtQtQtQtQt",
"QtQtQtQtQtQtQt.bcpcycz.Daj.V#u#CcAcBbH#i#u#u.4aBaccCcDcE.bQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQt.aaecFcGcHcIcJcK.J.GcLcMcNcOcP.qcQcRcS#qQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtcT.a.b.QcUcVcWcXcYcZc0cYc1c2c3c4cS.b.aQtQtQtQtQtQtQtQt",
"c5c5c5c5c5coQtQtQtQtQt.a.bc6c7c8c9d.d#dadbdc.bc5c5coQtc5c5c5c5c5coQtQt",
"dddedfdedgdhc5cocoQtcococo#q.bc5.bc5.bc5c5.b.#didjdkcodldmdndodpdqcoQt",
"drdsdtdudvdec5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5dwdxdyc5dzdAdBdCdDdmc5c5",
"dEdFdGdHdIdJdKdLdMdNdOdPdQdRdSdTdUdVdWdXdYdZd0d1d2d3c5d4d5d6d7d8d9dOdg",
"Qte.e#eaebdFecedeeefegeheiejekelemenefeoepeqereseteuc5evewexeyezeAeBeC",
"c5eDeEeFeGeHeIeJeKeLeMeNeOdyePeQeReSeTeUeVeWeXeYeZe0c5dEe1e2e3e4e5e6e7",
"c5e8eEe9f.f#fafbfcfdfefffgc5fhfifjfkflfmfnfofpeYfqfr.#fsftfufvfefwfxfy",
"c5fzehfAfBfCfDfEfFfGfHfIfJfKfLfMfNfOfPfQfDfReXfSfTfUfVeJfWfXfYfZf0f1f2",
"c5f3f4f5Qtf6f7f8f9g.g#gagbQtgcgdfjgedXgfggghgigje6gkglgmfvgngogpgqgrgs"};

static const char* const image3_data[] = { 
"30 30 585 2",
"gB c #146a9d",
"gD c #146ca1",
"gh c #1569a4",
"f9 c #156b9d",
"gC c #166a9c",
"gi c #1769a4",
"aV c #176ba0",
"fk c #176ba6",
"gE c #176ca1",
"fK c #186ca1",
"fi c #186d9e",
"f8 c #186e9f",
"gg c #19689c",
"bK c #19699e",
"gm c #196aa0",
"au c #196aa2",
"eZ c #196b9f",
"gn c #196ba4",
"av c #196ca5",
"fI c #196f9c",
"fJ c #19709f",
"gF c #1a6a9f",
"ef c #1a6aa0",
"ei c #1a6ba5",
"fH c #1a6da0",
"c# c #1b679c",
"eE c #1b699d",
"gG c #1b699e",
"b8 c #1b6a9d",
"dr c #1b6aa1",
"cy c #1b6b9f",
"bg c #1b6ba1",
"eW c #1b6c9f",
"bj c #1b6ca0",
"f5 c #1c6ba5",
"c3 c #1c6ca1",
"dS c #1c6ca4",
"go c #1c6ca6",
"c1 c #1c6da1",
"b9 c #1c6da4",
"dQ c #1c6ea2",
"fj c #1c6f9e",
"fU c #1d6698",
"bH c #1d689c",
"dp c #1d6a9b",
"gl c #1d6a9c",
"gr c #1d6aa0",
"aY c #1d6ba0",
"gq c #1d6ba2",
"gp c #1d6ca6",
"dq c #1d6da2",
"eY c #1d6da3",
"f6 c #1d6da4",
"aW c #1d6ea1",
"c2 c #1d6ea3",
"gj c #1e6aa1",
"eX c #1e6ca0",
"eg c #1e6da0",
"bJ c #1e6da1",
"cz c #1e6da3",
"f7 c #1e6e9f",
"dR c #1e6ea2",
"gA c #1e6f9d",
"gk c #1f6a9d",
"eC c #1f6a9e",
"bI c #1f6ca1",
"fG c #1f6ca3",
"eh c #1f6da1",
"c. c #1f6da3",
"aX c #1f6ea3",
"bh c #206d9e",
"bi c #206e9f",
"cA c #21679a",
"ft c #216994",
"eD c #216c9e",
"gs c #226c9e",
"c0 c #236c9d",
"fh c #246e9e",
"gH c #246ea0",
"fL c #266c9f",
"fV c #266d9d",
"dP c #2773a3",
"gf c #286e9c",
"aw c #296d9c",
"#7 c #296e9f",
"f4 c #296ea0",
"g. c #29719f",
"at c #2a709b",
"eV c #2b729f",
"aZ c #2c6e9b",
"c4 c #2d6d9b",
"aU c #2f75a0",
"ej c #2f76a7",
"e6 c #3074a1",
"fl c #3075a1",
"#6 c #33739f",
"cx c #3378a6",
"gS c #397fa6",
"e0 c #3a7ba0",
"bk c #3a7ca1",
"f3 c #3c7ca8",
"gT c #3c7ea4",
"gI c #3e7aa3",
"gR c #4083a6",
"bf c #4083ad",
"ds c #417da6",
"gU c #4481a5",
"ee c #4583ab",
"b7 c #4685ab",
"fW c #4985af",
"gz c #4986a6",
"fF c #4d7898",
"gt c #4e85aa",
"gV c #4f89a9",
"bL c #508aaa",
"gQ c #518ea9",
"eJ c #5283ab",
"bG c #568fb1",
"fu c #598fae",
"do c #5a93b3",
"fT c #5b91b5",
"db c #5c6165",
"ca c #5c8fb3",
"f2 c #6196b9",
"da c #62686a",
"fg c #628ca7",
"e7 c #659ab6",
"en c #668fb0",
"eF c #6798b7",
"eK c #6997bc",
"cN c #6a6a6b",
"gW c #6b9eba",
"cI c #6c6b6a",
"cM c #6c6b6c",
"de c #6c6e6f",
"ge c #6c9ab8",
"fs c #6c9fbb",
"dT c #6da2c2",
"eB c #6e9ebb",
"df c #6f7173",
"ax c #6f9db9",
"cJ c #707477",
"gP c #70a3ba",
"fX c #71a1c1",
"cB c #729dba",
"d# c #737575",
"dc c #73777b",
"cK c #777d82",
"cL c #797d80",
"cO c #7b7c7e",
"gJ c #7da5be",
"eU c #7da8be",
"d. c #7e7c7d",
"g# c #7ea8c2",
"fM c #80a8c1",
"as c #80acc4",
"#8 c #81a8c1",
"f1 c #82aac1",
"#L c #86abc1",
"fY c #86aec6",
"fE c #8c8682",
"dX c #8eabbc",
"ce c #8eacbe",
"a0 c #8eb1c5",
"f0 c #8fb2c6",
"fZ c #8fb4c9",
"dB c #909192",
"#5 c #90b3c6",
"cZ c #90b8cd",
"dd c #919496",
"gy c #91b9ca",
"cF c #92b1c2",
"bw c #94b1bc",
"e5 c #94b9d1",
"dO c #94bace",
"bB c #97b8c7",
"gX c #97bdd1",
"fm c #97bece",
"dF c #989c9e",
"gu c #98bbce",
"gO c #98bfcf",
"cH c #999695",
"c5 c #99b7ca",
"aT c #99b9c7",
"#K c #9abbcc",
"b2 c #9bb9ca",
"em c #9ebbcf",
"dy c #9fa0a3",
"bY c #9fbec7",
"bP c #a2bdc7",
"fv c #a4c4d4",
"bl c #a5c4d1",
"dA c #a7aeae",
"dz c #aab0b2",
"dj c #aca597",
"cp c #adc1cc",
"dY c #adc2d0",
"be c #adc9d3",
"ff c #ae947e",
"cT c #aec8cd",
"dw c #afc6cf",
"ek c #afd0e0",
"ct c #b0c3cc",
"aE c #b0c4ca",
"a5 c #b0c5cd",
"bp c #b0c7cf",
"aa c #b0c8d4",
"cS c #b0c9cd",
"c8 c #b0cad4",
"aP c #b0cad6",
"#F c #b1c3cb",
"cR c #b1cad0",
"ba c #b2cbd7",
"cd c #b2cfd9",
"#j c #b3c3c9",
"fD c #b49781",
"dC c #b4b6b7",
"ai c #b4c5ce",
".3 c #b4cad2",
".W c #b5c8d1",
"e1 c #b5d0db",
"co c #b6c4c8",
"aK c #b6c6cc",
"bC c #b6cacf",
".4 c #b6ccd0",
"ao c #b6cdd4",
"cl c #b7b5b3",
"ae c #b7c9d1",
"#u c #b7c9d3",
"## c #b7ccd6",
"bx c #b7cfd3",
"cE c #b7d3de",
".X c #b8cbd3",
"aB c #b9ccd4",
"bo c #b9ccd5",
"bQ c #b9cfd6",
"#a c #bacad2",
"#R c #bacad6",
"bZ c #bad4db",
"ad c #bbc7cd",
"a3 c #bbcbd2",
"cw c #bbd8e3",
"#Q c #bccdd8",
"bO c #bcd3da",
"ah c #bdcfd9",
".K c #bfd1d5",
"dt c #bfd4dd",
"#1 c #c0ced6",
"a8 c #c0d0d6",
"#v c #c0d1da",
"af c #c2d5de",
"dx c #c3c6cc",
"dE c #c4c8ca",
"a9 c #c4d3d8",
".J c #c4d3d9",
".2 c #c4d5dd",
"ed c #c4dde7",
"ch c #c5c2c3",
".L c #c5d4da",
"ag c #c5d9e1",
"gY c #c5dbe4",
"g7 c #c7e0e8",
"cn c #c8cfd0",
"aF c #c8d4da",
"bq c #c8d9e1",
"cP c #c9cac9",
".Y c #c9d7e0",
".V c #c9dbe1",
"b3 c #c9dce2",
"g8 c #c9e0e7",
"ck c #cbcbcc",
"cj c #cccfd1",
"bn c #ccdfe4",
"aC c #cce0e7",
"c9 c #cdd0d3",
".I c #cdd9e0",
".M c #cddae1",
"cq c #cddde3",
"e8 c #cde3e8",
"eT c #cec3b4",
"ci c #ced0d1",
"gK c #cee1e9",
"dk c #cfcec5",
"#i c #cfdde0",
"eo c #cfdfea",
"a4 c #cfe0e5",
"c7 c #cfe3ea",
"#V c #d0dde2",
"gd c #d0e2eb",
"g6 c #d0e5ea",
"aJ c #d1dee1",
"cf c #d1e0e5",
"a6 c #d2dfe2",
"#k c #d2e0e5",
"eI c #d2e2ec",
"g9 c #d2e4eb",
"fe c #d3864a",
"#2 c #d3dcdf",
".1 c #d3e0e5",
".Z c #d3e1e6",
"fw c #d3e7ec",
"#U c #d4e0e4",
"an c #d4e1e6",
"fS c #d4e4eb",
"eO c #d58245",
"ey c #d58447",
"di c #d5cfc3",
"cm c #d5d6d5",
"cu c #d5e1e7",
"aj c #d5e2e6",
"a# c #d5e2e7",
"b1 c #d5e3ea",
"b6 c #d5e8ed",
"dn c #d5e8ef",
"fC c #d6b9a0",
"eu c #d77d3f",
"ea c #d7905d",
"cg c #d7dad9",
"cG c #d7dfe2",
"cs c #d7e5e6",
"d8 c #d8844c",
".0 c #d8e3e6",
"cU c #d8e6e7",
".5 c #d8e6e8",
"dJ c #d98750",
"dg c #d9dada",
"#W c #d9e3e8",
".N c #d9e7eb",
"dK c #da9767",
"ay c #daeaef",
"eS c #db8b50",
"#E c #dbe3e7",
"bv c #dbe4e9",
"fc c #dc8644",
"dI c #dc9462",
"#9 c #dceaee",
"h. c #dcecf0",
"gx c #dcedf0",
"e# c #dd7b3a",
".H c #dde6eb",
"aD c #ddebed",
"fr c #ddebf0",
"ex c #de7d3c",
"eP c #de7e38",
"fd c #de823a",
"fb c #dea87b",
"eL c #deebf1",
"bM c #deedf0",
"#M c #deeef1",
"d9 c #df7935",
"a2 c #dfedef",
"g5 c #dfeeee",
"ev c #e07d3b",
"ew c #e07e3e",
"#T c #e0e9ec",
"aO c #e0eaef",
"bF c #e0f1f3",
"eR c #e17c33",
"eQ c #e17e34",
"e. c #e27b39",
"dG c #e2dcd4",
"ac c #e2e7eb",
"eA c #e2f0ef",
"ar c #e2f0f5",
"bX c #e2f1f5",
"et c #e3b998",
"#b c #e3e8ec",
"cQ c #e3eef1",
"dU c #e4f1f4",
"ga c #e5eff3",
"dv c #e5f0f1",
"eG c #e5f3f5",
"aL c #e6ecf2",
"b# c #e6eef2",
"gv c #e6f3f6",
"dL c #e7c7ad",
"ab c #e7f1f5",
"fN c #e7f3f6",
"dH c #e8c1a3",
"ez c #e8ceb0",
"bd c #e8f0ed",
"bA c #e8f1f4",
"#. c #e8f1f5",
"cb c #e8f3f5",
"eN c #e9c5a5",
"d7 c #e9cbb2",
"#4 c #e9f4f5",
"aS c #eaf1f1",
"h# c #eaf7f6",
"bb c #ebf4f1",
"gZ c #ebf4f5",
"#q c #ebf5f8",
".U c #ebf6f8",
"aG c #ecf1f4",
"#t c #ecf3f2",
"#J c #ecf4f7",
"#X c #edf1f4",
".O c #edf9f9",
"fn c #edf9fa",
"eb c #eeddc9",
"#G c #eef2f3",
"bm c #eef3f4",
"dW c #eef4f3",
".G c #eef5f7",
"fB c #efe5d4",
"dD c #eff2f4",
"a1 c #eff3f5",
"fx c #eff7f6",
"bD c #f0f5f2",
"g4 c #f0f7f6",
"gN c #f0f9f7",
"aQ c #f0f9f8",
"bR c #f0fafb",
"#p c #f1f8f9",
"dZ c #f2f5f5",
"#h c #f2f8f9",
"cC c #f2f9fa",
"b0 c #f2fbfa",
"ap c #f2fbfb",
"d6 c #f3f7f4",
"aI c #f3f9fa",
"cY c #f3fafc",
"#S c #f4f5f8",
"#w c #f4f9f8",
"gL c #f4fbfc",
"cr c #f4fcf9",
"#c c #f5f7f8",
"#P c #f5f9f9",
"b4 c #f5f9fa",
"aA c #f5faf8",
".v c #f5faf9",
"fa c #f6e7d6",
"d2 c #f6f7f2",
"d0 c #f6f8f8",
"fA c #f6fbf5",
"by c #f6fbf8",
"aH c #f6fbfc",
"g1 c #f6fdfd",
"d1 c #f7f9f8",
"#0 c #f7f9fb",
"a7 c #f7fafb",
"dl c #f7fbf4",
"fy c #f7fbf7",
".6 c #f7fbfb",
"fP c #f7fcfd",
"fz c #f7fdf9",
".F c #f7fdfc",
"fo c #f7fffe",
"e4 c #f8faf8",
"b. c #f8fafb",
".u c #f8fbfb",
"fp c #f8fcfc",
"e3 c #f8fdfb",
"el c #f8fefe",
"e9 c #f8fffd",
"d3 c #f9faf7",
".w c #f9fafc",
"#s c #f9fcfa",
"#l c #f9fcfc",
".a c #f9fdfd",
"e2 c #f9fffc",
"dM c #faf6ed",
"#r c #fafbfb",
"ak c #fafbfd",
"gM c #fafcf9",
".9 c #fafcfc",
"fQ c #fafcfd",
"hc c #fafdfb",
".c c #fafdfc",
".b c #fafdfd",
"fO c #fafdfe",
"ha c #fafefc",
"#D c #fafefd",
"d5 c #fafefe",
".P c #fafeff",
"aM c #fbfafb",
"#B c #fbfbf8",
"#I c #fbfbf9",
".R c #fbfbfa",
"#O c #fbfbfb",
"#g c #fbfbfc",
"bc c #fbfcf5",
"#H c #fbfcf9",
"cV c #fbfcfa",
".7 c #fbfcfb",
".B c #fbfcfc",
"gc c #fbfcfd",
".o c #fbfdfa",
".A c #fbfdfb",
".d c #fbfdfc",
".p c #fbfdfd",
"g3 c #fbfefb",
".z c #fbfefc",
"aq c #fbfefd",
"c6 c #fbfefe",
"bu c #fcfaf8",
"br c #fcfafa",
".E c #fcfafb",
"fR c #fcfafc",
"bt c #fcfbf7",
"#A c #fcfbf8",
"bT c #fcfbfa",
"al c #fcfbfb",
".Q c #fcfbfc",
"bs c #fcfcf9",
"#f c #fcfcfa",
"#Z c #fcfcfb",
".h c #fcfcfc",
".C c #fcfcfd",
"dh c #fcfdf7",
".n c #fcfdf8",
"#e c #fcfdfa",
".j c #fcfdfb",
"#n c #fcfdfc",
".# c #fcfdfd",
".T c #fcfdfe",
"dN c #fcfef7",
"#d c #fcfefb",
"g0 c #fcfefc",
"#C c #fcfefe",
"ec c #fcfff9",
"bE c #fcfffc",
"b5 c #fcfffe",
"cv c #fcffff",
".D c #fdfafb",
".S c #fdfafc",
"gw c #fdfbf6",
"cW c #fdfbf9",
".q c #fdfbfb",
"hg c #fdfbfd",
"az c #fdfcf8",
"hb c #fdfcfa",
"#m c #fdfcfb",
"#o c #fdfcfc",
"#Y c #fdfcfd",
".g c #fdfdf7",
"Qt c #fdfdf8",
".i c #fdfdf9",
".f c #fdfdfa",
".e c #fdfdfb",
".l c #fdfdfc",
".k c #fdfdfd",
".x c #fdfdfe",
"es c #fdfef8",
"#N c #fdfef9",
"#3 c #fdfefb",
"eM c #fdfefc",
".t c #fdfefd",
"eH c #fdfefe",
"er c #fdfffc",
"bW c #fdfffe",
"d4 c #fdffff",
"cX c #fefafb",
"bU c #fefbf7",
"fq c #fefbf9",
"#z c #fefcf8",
"a. c #fefcfc",
"f. c #fefdf7",
"aR c #fefdf9",
"bz c #fefdfb",
".m c #fefdfc",
".8 c #fefdfd",
"am c #fefdfe",
"bS c #fefdff",
"du c #fefefb",
".r c #fefefc",
"dm c #fefefe",
"f# c #fefff8",
"he c #fefffb",
"bN c #fefffc",
".y c #fefffd",
"bV c #fffcf8",
"ep c #fffcfc",
"#y c #fffdf9",
"eq c #fffdfa",
"gb c #fffdfb",
"cD c #fffdfd",
"dV c #fffefa",
"hd c #fffefb",
"cc c #fffefd",
"aN c #fffefe",
"hf c #fffffb",
"g2 c #fffffc",
"#x c #fffffd",
".s c #fffffe",
"Qt.#.a.a.b.b.c.d.e.f.g.f.h.f.i.j.#.k.k.k.k.k.k.k.k.k.k.k.k.k",
".e.#.#.#.l.e.f.e.m.m.m.k.k.f.n.o.p.k.k.k.k.k.k.k.k.k.k.k.k.k",
".b.#.h.q.f.f.r.s.t.u.v.w.x.y.z.A.B.C.k.k.k.k.k.k.k.k.k.k.k.k",
".a.#.D.E.d.F.G.H.I.J.K.L.M.N.O.P.Q.R.h.k.k.k.k.h.h.k.k.k.k.k",
".b.C.S.T.U.V.W.X.Y.Z.0.1.2.3.4.5.6.7.h.k.k.k.k.h.8.s.8.k.k.k",
".9.9.7#.###a#b#c.A#d#e#f#g#h#i#j#k#l#m.j#n#o.l#n#p#q.b.e.l#o",
"#r#s#t#u#v#w#x#y#z#z#A#B.h#C#D#E#F#G#z#B#H#I.r#J#K#L#M#N.R#O",
".Q#P#Q#R#S#P#T#U#V#W#X#l.C#Y#Z#0#1#2#Z.7#Z#3#4#5#6#7#8#9.A#I",
"a.a#aaabacadaeafagahaiajak.Dalamanaoap#oaqarasatauavawaxayaz",
"aAaBaCaDaEaFaGaH.aaIaJaKaLaM#maNaOaPaQaRaSaTaUaVaWaXaYaZa0a1",
"a2a3a4a5a6.w.h#Z.7#na7a8a9b..daNb#babbbcbdbebfbgbhbibjbkblbm",
"bnbobpbq.wbrbsbtbu.Bbvbwbxby.RbzbAbBbCbDbEbFbGbHbIbJbKbLbMbN",
"bObPbQbRbS#YbTbUbVbWbXbYbZb0.R#rb1b2b3b4b5b6b7b8b9c.c#cacbcc",
"cdcecfcgchcicjckclcmcncocpcqcrcsctcu.6alcvcwcxcyb9czcAcBcCcD",
"cEcFcGcHcIcJcKcLcMcNcOcPcQcRcScTcUcVcWcXcYcZc0c1c2c3c4c5c6.k",
"c7c8c9d.d#dadbdcdddedfdgdhdidjdkdl#H#Bdmdndodpdqc2drdsdtdu#o",
"dvdwdxdydzdAdBdCdDdEdFdGdHdIdJdKdLdMdN.FdOdPdQdRdSdrdTdUdV.h",
"dWdXdYdZd0d1d2d3d4d5d6d7d8d9e.e#eaebecedeeefegeheiejekel.h.k",
"#remeneoepeqer.t.#bWeseteuevewexeyezeAeBeCaYeDdqeEeFeGer#n.k",
"eHeIeJeKeLbNaq.p.heM.neNeOePeQeReSeTeUeVeWeXeYeZe0e1e2.o.l.k",
"e3e4e5e6e7e8e9#xbVf.f#fafbfcfdfefffgfhfifjdQfkflfmfnfofp#o.k",
"#rfqfrfsftfufvfwfxfyfzfAfBfCfDfEfFfGfHfIfJfKfLfMfNfOfPfQ#Y.k",
"fR.B.efSfTfUfVfWfXfYfZf0f1f2f3f4f5f6f7f8f9g.g#gagbalgc.h.k.k",
"fQ.b#m#3gdgegfggghgigjgkglgmgngogpgqgrgsgtgugvbW#o.k.k.k.k.k",
"fQgcbsgw#egxgygzgAgBgCf9gDgEgFeEgGgHgIgJgKgL.dal.C.k.k.k.k.k",
"#Y#Y#ngM#H#HgNe8gOgPgQgRgSgTgUgVgWgXgYgZg0.B.9#o.k.k.k.k.k.k",
".#.k.bg1fp.d.mg2g3g4g5g6g7g8g9h.h#hag2eqhb.h.#.k.k.k.k.k.k.k",
".b.j#ffpfp.9hc.7.idVhddVhehf#x#x.r.l#m.q.hhg#Y.k.k.k.k.k.k.k",
".p.l#m.l.k.l.A.#.#.l#mhbbz.m.k.l#n.k.k.k#Y#Y#Y.k.k.k.k.k.k.k",
".k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k.k"};

static const char* const image4_data[] = { 
"90 30 1222 2",
"ay c #4e819d",
"ld c #55819c",
"ax c #5587a3",
"hO c #567c8e",
"az c #5a89a1",
"#B c #5a8ca8",
"hU c #5b8297",
"#C c #5b8ca7",
"#A c #5b8ca8",
"lW c #5c859f",
"#z c #5c8ca8",
"#y c #5c8da8",
"hN c #5e869c",
"lj c #5e89a4",
"#x c #5e8ea9",
"pP c #5f849c",
"h5 c #5f88a0",
"iM c #5f8ba3",
"o9 c #6089a3",
"#D c #618da6",
"p. c #63879f",
"#I c #638ea8",
"#J c #638fa7",
"aw c #6391a9",
"iN c #64889a",
"iT c #65889a",
"h6 c #65899f",
"iS c #658fa6",
"#K c #6590a8",
"aA c #6592a8",
"i4 c #668aa2",
"bx c #6690a4",
"q. c #6691ab",
"#L c #6791a9",
"oC c #688ba4",
"le c #688ca5",
"hT c #6890a6",
"ql c #6893ac",
"av c #6894aa",
"aB c #6894ab",
"qk c #6894ad",
"km c #698aa1",
"gF c #6993ab",
"au c #6995ac",
"kx c #6a91a9",
"p9 c #6a93ae",
"jL c #6b93a9",
"#M c #6b93ac",
"pQ c #6c8ca1",
"aC c #6c94a8",
"g0 c #6c95ac",
"aI c #6d8d9f",
"lk c #6d8ea2",
"ln c #6d8fa4",
"qm c #6d93a9",
"gC c #6d93aa",
"q# c #6d94a9",
"#N c #6d95ae",
"p8 c #6d95af",
"gM c #6e8b9b",
"jW c #6e90a7",
"py c #6e92a7",
"li c #6e95ae",
"gE c #6e98b0",
"aH c #6f8ea0",
"l7 c #6f8ea1",
"aJ c #6f8fa0",
"o# c #6f91a8",
"iY c #6f93a7",
"g1 c #6f96aa",
"pK c #6f96ae",
"mP c #708e9f",
"kt c #7091a4",
"pu c #7095ad",
"fE c #709ab2",
"o1 c #718e9f",
"aK c #718fa0",
"i3 c #719ab0",
"gL c #7290a2",
"pO c #7295aa",
"hJ c #7297aa",
"o5 c #7297ad",
"pD c #7390a1",
"aL c #7391a2",
"gN c #7395a8",
"lX c #7395ab",
"gO c #7396aa",
"or c #7397aa",
"gV c #7397ab",
"#O c #739ab2",
"mG c #7492a4",
"g5 c #7495a8",
"gU c #7497aa",
"kB c #7499ac",
"h0 c #749ab0",
"fF c #749bb3",
"f0 c #749cb2",
"lf c #7597b0",
"oX c #7599af",
"ks c #759ab0",
"gB c #759ab1",
"lV c #759bb3",
"aM c #7693a4",
"jz c #7693a8",
"gT c #7697a9",
"md c #7699ae",
"mV c #769aae",
"gD c #769bb0",
"#H c #769bb3",
"lc c #769db4",
"nZ c #7796a6",
"kJ c #7797ac",
"nB c #7797ad",
"h1 c #7798aa",
"lh c #7798b1",
"lg c #7798b2",
"iD c #7799ae",
"lv c #779bb0",
"iI c #779cb1",
"#w c #779fb6",
"hE c #7898ad",
"g4 c #789bac",
"mF c #789cb2",
"#P c #789db5",
"fZ c #789fb5",
"oB c #789fb6",
"aN c #7995a7",
"cm c #799baa",
"g6 c #7a98ab",
"oA c #7a99ae",
"nx c #7a9eb2",
"#Q c #7a9eb6",
"#R c #7a9fb6",
"jI c #7b9aab",
"lr c #7b9caf",
"cP c #7b9dac",
"nl c #7c97a5",
"aO c #7c97a9",
"jR c #7c9bac",
"hP c #7d99a7",
"aP c #7d99aa",
"lz c #7d9baf",
"jF c #7d9cad",
"ps c #7d9fad",
"nk c #7da0b3",
"kF c #7da0b5",
"#S c #7da1b7",
"bw c #7da2b7",
"m0 c #7e9aaf",
"gS c #7e9eb1",
"g3 c #7ea2b6",
"fU c #7ea2b7",
"aQ c #7f99a9",
"hV c #7f9ba9",
"m# c #7f9eb1",
"fD c #7fa5ba",
"qj c #7fa5bb",
"aR c #809aaa",
"mi c #809cb0",
"jM c #809daa",
"at c #80a4b7",
"fN c #80a4b8",
"qn c #819cad",
"fO c #81a0b2",
"nt c #81a0b3",
"lY c #81a0b5",
"l2 c #81a1b6",
"l1 c #81a2b6",
"kl c #81a2b7",
"n6 c #81a3b8",
"g2 c #81a4b7",
"#T c #81a4bb",
"lZ c #82a2b6",
"l0 c #82a2b7",
"jE c #82a5b8",
"hI c #82a7bb",
"aS c #839cad",
"f1 c #83a5b7",
"oU c #84a1b2",
"pG c #84a1b4",
"hD c #84a5b9",
"aT c #859eae",
"aG c #859fb0",
"fV c #85a3b5",
"#U c #85a7be",
"pJ c #85a8bc",
"oY c #86a0b0",
"aU c #87a0af",
"#E c #87a2b1",
"jQ c #87a8bb",
"#V c #87a9bf",
"n7 c #88a3b4",
"pv c #88a5b9",
"nY c #88a8bb",
"aV c #89a1af",
"aD c #8aa3b2",
"gP c #8aa4b3",
"kG c #8aa4b4",
"#W c #8aa9be",
"aW c #8ba1af",
"qN c #8baabb",
"#X c #8baabe",
"h4 c #8babbb",
"p7 c #8bacc0",
"iJ c #8ca6b5",
"qA c #8caabe",
"aX c #8da3b1",
"gW c #8da8b7",
"jV c #8dadc1",
"ky c #8ea6b4",
"qo c #8ea6b5",
"by c #8eabb7",
"qB c #8eabbb",
"pL c #8eacbe",
"oz c #8eadc0",
"gG c #8faab9",
"ov c #90a8b6",
"ny c #90a9b7",
"#Y c #90acbf",
"aY c #91a5b2",
"lw c #91a8b7",
"o6 c #91abb9",
"aZ c #92a6b2",
"qc c #92a7b4",
"fG c #92aebf",
"jy c #92afc1",
"pt c #92b1c3",
"o. c #92b1c4",
"mW c #93abb7",
"pN c #93b1c3",
"a0 c #94a8b3",
"me c #94a9b7",
"#Z c #94b0c2",
"l3 c #95aabb",
"qa c #95adba",
"a1 c #96a9b4",
"a2 c #97aab4",
"#0 c #97b2c3",
"iX c #97b4c5",
"lo c #99aab6",
"kn c #99acbb",
"kC c #99afba",
"p5 c #99b2bd",
"o8 c #99b5c6",
"qM c #99b7c7",
"a3 c #9aacb6",
"#1 c #9ab4c4",
"cl c #9ab7c6",
"gZ c #9ab7c8",
"fY c #9ab8c9",
"a5 c #9badb6",
"a4 c #9badb7",
"ou c #9bb6c5",
"px c #9bb8ca",
"a6 c #9caeb7",
"iE c #9cafbd",
"#2 c #9cb6c5",
"oq c #9cb8c8",
"qO c #9db3c0",
"qf c #9eb4c1",
"#3 c #9eb7c5",
"d. c #9ebac8",
"l8 c #9facb4",
"cO c #9fbac8",
"a7 c #a0b0b9",
"iZ c #a0b2bd",
"#7 c #a0b8c7",
"mQ c #a1aeb5",
"#4 c #a1b9c8",
"pr c #a1bbc9",
"nr c #a2aeb5",
"o2 c #a2aeb6",
"#6 c #a2bac9",
"fT c #a2bbca",
"qz c #a2bbcc",
".J c #a2bece",
"a8 c #a3b3bb",
"#5 c #a3bac8",
"fM c #a3bbc9",
"kI c #a3bdcd",
".I c #a3bfcf",
"pE c #a4b0b7",
"#8 c #a4bbc9",
"f2 c #a4bcc6",
"pM c #a4bdcd",
"pz c #a5b5c1",
"hF c #a5b7c3",
"pw c #a5bccc",
"nA c #a6bece",
"o4 c #a6bfce",
"a9 c #a7b5bd",
"mA c #a7b6be",
"ls c #a7b9c2",
"os c #a7b9c3",
".K c #a7bfce",
"iu c #a8b7c0",
"nH c #a8b7c1",
"iO c #a8b9c0",
"hK c #a8bac4",
"qC c #a8bdc7",
"kr c #a8bece",
"fC c #a8c2d0",
"m3 c #a9b7bf",
"mv c #a9b7c0",
"nG c #a9bbc6",
"iU c #aabac0",
"nb c #aabac3",
"#9 c #aabfcc",
"iC c #aac2d1",
"gA c #aac3d1",
"oD c #abb9c4",
"ht c #abbbc4",
"nf c #abbbc5",
"h2 c #abbdc6",
"gR c #abc0cc",
"mO c #abc1ce",
"ox c #abc1cf",
"l6 c #abc2cf",
"lm c #abc2d0",
"bv c #abc3cf",
"hM c #abc3d0",
"kw c #abc3d2",
"iL c #abc4d2",
".P c #abc4d3",
"i5 c #acb9c4",
"nC c #acb9c5",
"b. c #acbac4",
"b# c #acbcc6",
"fK c #acbcc7",
"#G c #acc0cf",
".Q c #acc5d3",
"kK c #adb9c5",
"h7 c #adbac5",
"ma c #adbcc5",
"mH c #adbcc6",
"fP c #adbdc6",
"pC c #adc2cf",
".R c #adc5d4",
".S c #adc6d4",
"mw c #aeb8be",
"lA c #aebac5",
"mj c #aebbc5",
"m4 c #aebcc4",
"nT c #aebdc7",
"a. c #aec2ce",
"hS c #aec4d1",
"pR c #afbcc6",
"mR c #afbec7",
"fL c #afbfc7",
"ly c #afc6d3",
".T c #afc7d5",
"dz c #b0bbc2",
"ol c #b0bcc4",
"nu c #b0bec7",
"nR c #b0bfc9",
"hW c #b0c0c9",
"mZ c #b0c5d4",
"lP c #b1bcc3",
"jr c #b1bdc4",
"lT c #b1bdc5",
"pH c #b1bfc8",
"hZ c #b1c7d4",
".U c #b1c9d7",
"iv c #b2bbc0",
"hj c #b2bdc3",
"mz c #b2bfc7",
"gQ c #b2c0c9",
"f4 c #b2c4cf",
"a# c #b2c5d1",
"gK c #b2c7d3",
"mh c #b2c7d5",
"hH c #b2c8d5",
"#v c #b2c8d6",
"kA c #b2c9d5",
"iH c #b2c9d6",
"ba c #b3bdc3",
"i. c #b3bdc5",
"ga c #b3bec4",
"jq c #b3bfc7",
"jA c #b3bfc8",
"jg c #b3c2cc",
"cn c #b3c4c9",
"j5 c #b3c5d2",
"bz c #b3c8d0",
"lq c #b3c8d5",
".H c #b3c9d7",
".O c #b3cad8",
"hc c #b4bdc3",
"ih c #b4bec4",
"m5 c #b4bec5",
"lO c #b4c0c7",
"mo c #b4c0c8",
"oj c #b4c1c8",
"g7 c #b4c1ca",
"e# c #b4c2ca",
"fR c #b4c6d1",
"f3 c #b4c7d0",
"m. c #b4c9d5",
".V c #b4cbd9",
"m9 c #b5bec5",
"b4 c #b5c1c9",
"lN c #b5c2c9",
"ja c #b5c2ca",
"gr c #b5c3ca",
"ib c #b5c3cb",
"ab c #b5c5cf",
"fS c #b5c6d1",
"aa c #b5c8d3",
"nc c #b6bfc4",
"dd c #b6c0c5",
"h# c #b6c1c8",
"k9 c #b6c2ca",
"gm c #b6c3cb",
"as c #b6c9d6",
"pF c #b6cad6",
"qi c #b6ccd9",
"gw c #b7bfc4",
"js c #b7c0c7",
"mm c #b7c1c7",
"ft c #b7c2ca",
"ml c #b7c3ca",
"mu c #b7c4cd",
"jn c #b7c5cd",
"it c #b7c6ce",
"oT c #b7ccd9",
".W c #b7ceda",
"qd c #b8c1c6",
"i8 c #b8c4cb",
"hu c #b9c2c7",
"ke c #b9c3c9",
"jw c #b9c4ca",
"im c #b9c5cc",
"lF c #b9c6ce",
"pj c #b9c6cf",
"cx c #b9c6d0",
"aF c #b9c6d1",
"oM c #b9c7d0",
"kU c #b9c7d1",
"d# c #b9c8cc",
"gy c #b9c9cc",
"i2 c #b9ced9",
".X c #b9cedb",
"pU c #bac2c6",
"m2 c #bac7cf",
"cQ c #bac8cc",
"fk c #bac8d0",
"kk c #bac9cc",
"nh c #bac9cd",
"pi c #bac9d2",
"jm c #bacbd5",
"k. c #bacbd6",
"bu c #baced6",
"mc c #baceda",
".Y c #bad0dc",
"l. c #bbc2c6",
"nU c #bbc3c8",
"oP c #bbc4c9",
"iy c #bbc7ce",
"gh c #bbc8cf",
"hb c #bbc8d1",
"oF c #bbccd6",
"j6 c #bbccd7",
"bt c #bbcfd7",
"mU c #bbcfda",
"eI c #bbcfdb",
"q5 c #bcc3c8",
"mr c #bcc4ca",
"hl c #bcc5ca",
"k7 c #bcc7cc",
"ip c #bcc7ce",
"ku c #bcc8ce",
"hy c #bcc8cf",
"#F c #bcc8d0",
"fo c #bcc9d2",
"ko c #bcc9d6",
"cT c #bccad2",
"e2 c #bccfda",
"kb c #bdc4c9",
"ic c #bdc5c9",
"gi c #bdc6cb",
"kN c #bdc7ce",
"k3 c #bdc8cf",
"oe c #bdc9d0",
"mn c #bdc9d1",
"k# c #bdcad1",
"f. c #bdcbd3",
"eq c #bdcbd5",
"oK c #bdccd5",
"bA c #bdced5",
".Z c #bdd2de",
"nO c #bec6cb",
"fp c #bec7cd",
"f8 c #bec8cd",
"kg c #bec9cf",
"n0 c #becace",
"b3 c #becbd3",
"p# c #beccd5",
"kT c #becdd6",
"oJ c #becdd7",
"ep c #becdd8",
"h3 c #bed0da",
"j9 c #bed0db",
"lu c #bed1db",
"k4 c #bfc6cb",
"bG c #bfc7cc",
"bH c #bfc8cc",
"bF c #bfc8cd",
"hp c #bfc8ce",
"aE c #bfc8d0",
"lH c #bfc9ce",
"fx c #bfcacf",
"mq c #bfcbd3",
"gv c #bfccd2",
"ne c #bfcdd6",
".L c #bfced6",
"eo c #bfced9",
"j8 c #bfd1dc",
"qL c #bfd2dd",
"oh c #c0c6ca",
"q4 c #c0c7cc",
"bI c #c0c8cc",
"ac c #c0c8cd",
"kR c #c0c8ce",
"pk c #c0c9cf",
"er c #c0cbd2",
"ed c #c0ccd3",
"qg c #c0cdd3",
"dc c #c0cdd5",
"kq c #c0cdda",
"jf c #c0ced8",
"il c #c0cfd8",
"j4 c #c0d0da",
"j7 c #c0d1db",
"nw c #c0d3dd",
".0 c #c0d3e0",
"i9 c #c1c8cb",
"hz c #c1c8cd",
"bJ c #c1c9cd",
"cU c #c1c9ce",
"qp c #c1cad1",
"qP c #c1cdd3",
"je c #c1cdd4",
"fl c #c1cdd5",
"kQ c #c1ced6",
"ph c #c1d0d9",
"eJ c #c1d1dc",
".1 c #c1d4e0",
"om c #c2c7cb",
"bK c #c2c9ce",
"bL c #c2cace",
"kf c #c2cbd2",
"jS c #c2ccd3",
"hf c #c2cdd5",
"gX c #c2ced5",
"jY c #c2cfd7",
"hG c #c2d0d8",
"eR c #c2d2db",
".2 c #c2d5e0",
"bM c #c3cace",
"bN c #c3cacf",
"f# c #c3ccd0",
"pT c #c3ced4",
"ig c #c3cfd7",
"kp c #c3cfdb",
"fQ c #c3d1d8",
"hs c #c3d2da",
"lU c #c3d5df",
"fu c #c4cacd",
"q6 c #c4cace",
"lG c #c4cacf",
"bO c #c4cbd0",
"ev c #c4ccd2",
"k6 c #c4cfd5",
"oV c #c4cfd6",
"nN c #c4d0d6",
"k2 c #c4d0d7",
"f7 c #c4d0d8",
"jh c #c4d1d8",
"g# c #c4d1d9",
"eY c #c4d2db",
"e1 c #c4d5df",
"mE c #c4d5e0",
"lQ c #c5cbce",
"bP c #c5ccd0",
"ob c #c5cdd1",
"m8 c #c5d1d8",
"oL c #c5d2da",
"qe c #c5d5dd",
"kE c #c5d6e0",
".3 c #c5d6e1",
"bQ c #c6ccd0",
"bR c #c6cdd0",
"eD c #c6cfd4",
"pb c #c6d0d6",
"n2 c #c6d1d8",
"fH c #c6d3da",
"nF c #c6d3db",
"bs c #c6d5dc",
"bS c #c7cdd1",
"nm c #c7cfd2",
"jG c #c7d1d6",
"dy c #c7d4dc",
"nQ c #c7d5de",
"eX c #c7d6df",
"lb c #c7d8e1",
"i# c #c8cdce",
"bT c #c8cdd1",
"bU c #c8ced1",
"nK c #c8ced2",
"k8 c #c8d1d7",
"gd c #c8d2d9",
"j0 c #c8d5dd",
"hC c #c8d8e1",
"pI c #c8d8e2",
".4 c #c8d9e3",
"r. c #c9cdcf",
"q9 c #c9cdd0",
"bV c #c9ced1",
"bW c #c9cfd1",
"gs c #c9cfd2",
"kh c #c9d1d4",
"cy c #c9d1d5",
"kd c #c9d3da",
"p6 c #c9d4d7",
"h. c #c9d4dc",
"eQ c #c9d8e1",
"jD c #c9d8e2",
"n5 c #c9d9e2",
"bX c #cacfd1",
"fd c #cacfd2",
"ok c #cacfd3",
"bY c #cad0d2",
"lD c #cad1d6",
"jJ c #cad3d7",
"bB c #cad4db",
"ho c #cad5dc",
"nS c #cad7e0",
"nj c #cad9e3",
".5 c #cadae3",
"eH c #cadae4",
"bZ c #cbd0d2",
"mp c #cbd0d3",
"bE c #cbd2d6",
"ow c #cbd4d9",
"fz c #cbd5dc",
"fn c #cbd7de",
"e3 c #cbd8df",
"en c #cbd8e0",
"jl c #cbd9e1",
".6 c #cbdbe4",
"q8 c #cccfd2",
"b0 c #ccd0d3",
"jb c #ccd2d5",
"b2 c #ccd2d7",
"l4 c #ccd4d9",
"lS c #ccd6dc",
"mI c #ccd7dd",
"hi c #ccd7de",
"b1 c #cdd2d4",
"oG c #cdd2d5",
"iz c #cdd3d6",
"gl c #cdd8df",
"fj c #cdd9e1",
".7 c #cddce5",
"dA c #ced1d4",
"eA c #ced3d7",
"if c #ced5d8",
"hg c #ced5d9",
"jN c #ced6d9",
"hQ c #ced7db",
"lE c #ced9df",
"fm c #ced9e0",
"kV c #ced9e1",
"gq c #cedae2",
"gY c #cedce3",
".8 c #cedce5",
"oZ c #cfd6dc",
"hX c #cfd9de",
"jv c #cfdae0",
"j# c #cfdae1",
"mK c #cfdae2",
"mJ c #cfdbe1",
"n1 c #cfdbe2",
".N c #cfdce5",
"fX c #cfdde5",
"## c #cfdde6",
"gH c #d0d8dd",
"qF c #d0d9dd",
"fc c #d0dae1",
"nJ c #d0dbe2",
"fJ c #d0dce4",
".9 c #d0dde6",
"#. c #d0dee6",
"mx c #d1d5d7",
"b5 c #d1d6d9",
"qr c #d1d8dc",
"qb c #d1d9dd",
"f5 c #d1d9df",
"h9 c #d1dbe2",
"ji c #d1dce3",
"#a c #d1dee7",
"jP c #d1dfe7",
"r# c #d2d4d6",
"oS c #d2d5d7",
"jt c #d2d6d8",
"ii c #d2d7da",
"pc c #d2d8db",
"n8 c #d2d9de",
"qQ c #d2dadf",
"qx c #d2dce1",
"io c #d2dce2",
"lC c #d2dce3",
"e. c #d2dde4",
"fA c #d3d7d8",
"gb c #d3d7d9",
"pl c #d3d8da",
"q3 c #d3d8dc",
"k5 c #d3d9dd",
"qD c #d3dbe0",
"dP c #d3dce1",
"dO c #d3dce4",
"qI c #d3dde2",
"jk c #d3dee4",
"jj c #d3dee5",
"pg c #d3dfe6",
"ge c #d4dadd",
"cw c #d4dee5",
"oI c #d4dee6",
"oa c #d4dfe7",
"#b c #d4e0e8",
"oQ c #d5d7da",
"n. c #d5d8da",
"jx c #d5d9db",
"oN c #d5dadb",
"es c #d5dadd",
"p1 c #d5dade",
"n# c #d5dbde",
"fg c #d5dbdf",
"p0 c #d5dbe0",
"kH c #d5dcdf",
"o7 c #d5e0e8",
"nX c #d5e1e8",
".G c #d5e2e9",
"eE c #d6d9db",
"ew c #d6d9dc",
"bb c #d6dadb",
"mB c #d6dadc",
"fy c #d6dbdd",
"of c #d6dde0",
"iK c #d6dde1",
"dC c #d6dee2",
"oO c #d6dee3",
"kY c #d6e0e7",
"kW c #d6e2e9",
"m6 c #d7dadd",
"iq c #d7dcde",
"kc c #d7dde0",
"fw c #d7e0e5",
"kX c #d7e2e9",
"#c c #d7e3ea",
"jU c #d7e4ea",
"jo c #d8dddf",
"mL c #d8dfe5",
"oR c #d8e0e4",
"my c #d8e0e6",
"eS c #d8e1e4",
"hR c #d8e1e5",
"eK c #d8e1e7",
"ot c #d8e4ea",
"dD c #d9d7cf",
"iw c #d9dcdd",
"kO c #d9dde0",
"j1 c #d9dee0",
"kZ c #d9e1e5",
"ix c #d9e2e7",
"fs c #d9e2e8",
"#d c #d9e5eb",
"cC c #daae70",
"q7 c #dadee0",
"pW c #dae2e5",
"hY c #dae2e7",
"#f c #dae2e8",
"mk c #dae3e8",
"dN c #dae4ea",
"#e c #dae4eb",
"de c #dbddde",
"in c #dbdfe1",
"ka c #dbdfe2",
"ki c #dbe1e5",
"qX c #dbe2e7",
"i7 c #dbe3e9",
"ec c #dbe4ea",
"fB c #dbe5ec",
"hd c #dcdedf",
"kj c #dcdfe0",
"fq c #dce0e2",
"gj c #dce1e3",
"eu c #dce3e8",
"gg c #dce4ea",
"n9 c #dce7ed",
"eh c #ddd5c4",
"iF c #dde2e5",
".M c #dde4e8",
"eC c #dde5e8",
"is c #dde5ea",
"oE c #dde6ed",
"e0 c #dde7ed",
"c. c #dead66",
"cY c #deba86",
"cB c #deceb4",
"gx c #dee0e1",
"dQ c #dee3e6",
"kP c #dee5ea",
"br c #dee5eb",
"qw c #dee6eb",
"e9 c #dee6ec",
"iW c #dee7ed",
"ck c #dee8ed",
"b9 c #dfcaab",
"nd c #dfe2e3",
"mf c #dfe3e6",
"mX c #dfe4e6",
"eZ c #dfe5e9",
"he c #dfe7ec",
"cS c #dfe7ed",
"oW c #dfe8ee",
"cD c #e0a44d",
"dj c #e0cdaf",
"ng c #e0e2e3",
"fh c #e0e3e4",
"qG c #e0e4e5",
"i0 c #e0e4e8",
"od c #e0e8ed",
"ik c #e0e9ed",
"eW c #e0e9ee",
"dH c #e1ba7c",
"hm c #e1e3e4",
"hq c #e1e3e5",
"p2 c #e1e4e6",
"lR c #e1e5e8",
"ez c #e1e6e9",
"qW c #e1e6ea",
"pZ c #e1e7ea",
"hx c #e1e8ec",
"go c #e1e8ed",
"eP c #e1e9ee",
"ad c #e2e4e5",
"jZ c #e2e4e6",
"ms c #e2e5e5",
"eB c #e2e5e6",
"bC c #e2e5e9",
"pn c #e2e6e8",
"#g c #e2e6e9",
"lI c #e2e7ea",
"q1 c #e2e8eb",
"pa c #e2e8ed",
"gc c #e2e9ee",
"m1 c #e2eaee",
"c# c #e3a548",
"dG c #e3be80",
".A c #e3c490",
"dE c #e3d2b3",
"eg c #e3dccb",
"f9 c #e3e6e7",
"pX c #e3e6e8",
"bD c #e3e6e9",
"kD c #e3e7e9",
"qV c #e3e8ec",
"qJ c #e3e9eb",
"kS c #e3eaef",
"e4 c #e3ebed",
"op c #e3ecf0",
".B c #e4c38e",
"cX c #e4dac9",
"po c #e4e5e6",
"hv c #e4e6e7",
"dX c #e4e6e8",
"lp c #e4e7e8",
"qR c #e4e7ea",
"iP c #e4e8ea",
"n3 c #e4e9eb",
"ll c #e4eaee",
"qE c #e4ebef",
"j3 c #e4ebf0",
"eG c #e4edf2",
"cZ c #e5ab52",
"bg c #e5b56a",
"dF c #e5c792",
"bf c #e5d2b4",
"ei c #e5ddcd",
"di c #e5e5e0",
"qU c #e5e8e8",
"pq c #e5e8e9",
"dU c #e5e9ea",
"pA c #e5e9eb",
"jp c #e5e9ec",
"kv c #e5ebef",
"nM c #e5ecef",
"em c #e5ecf0",
"gz c #e5edf1",
"dk c #e6bb75",
"dI c #e6c284",
"ee c #e6e5df",
"l# c #e6e7e8",
"nV c #e6e8e9",
"d1 c #e6e9ec",
"pp c #e6eaec",
"ff c #e6ebed",
"fW c #e6ebef",
"l5 c #e6ecef",
"jX c #e6edf0",
"db c #e6edf1",
"cE c #e7a541",
"cF c #e7a640",
"cK c #e7c182",
".z c #e7d0aa",
"oi c #e7e8e8",
"gp c #e7e8e9",
"id c #e7e8ea",
"l9 c #e7e9ea",
"iV c #e7eaeb",
"qT c #e7eaec",
"qy c #e7ebee",
"gu c #e7edf1",
"o3 c #e7eef2",
"c9 c #e7eef3",
"cN c #e7eff3",
"ca c #e8a63f",
"cb c #e8a641",
"#o c #e8af56",
"#n c #e8b665",
"cV c #e8eaeb",
"hL c #e8ebed",
"ju c #e8ecee",
"og c #e8ecef",
"qY c #e8edef",
"jH c #e8edf1",
"f6 c #e8eef1",
"dM c #e8eef2",
"dB c #e8eef3",
"hr c #e8eff2",
"c0 c #e9a63e",
"cG c #e9a73f",
"bk c #e9a740",
"bh c #e9a944",
"#p c #e9af55",
"ah c #e9c58a",
"#m c #e9c68b",
"ra c #e9eaea",
"j. c #e9eaeb",
"hA c #e9ebeb",
"gn c #e9ebec",
"hn c #e9eef0",
"o0 c #e9eef2",
"qh c #e9eff3",
".a c #e9f0f3",
"c1 c #eaa63e",
"al c #eaa83f",
"dm c #eaab47",
"cJ c #eaac47",
"c4 c #eab35a",
"c5 c #eacd9e",
".C c #eacfa1",
"ns c #eaebec",
"na c #eaeced",
"jB c #eaecef",
"eN c #eaeef1",
"co c #eaeff0",
"pS c #eaeff2",
"g. c #eaeff3",
"nP c #eaf0f3",
".b c #eaf0f4",
"bj c #eba83d",
"ak c #eba83e",
"aj c #ebaa40",
"dl c #ebb151",
"ai c #ebb154",
"dp c #ebc68a",
"dJ c #ebd0a1",
"fa c #ebeced",
"d2 c #ebedee",
"k0 c #ebedef",
"mD c #ebeff0",
"k1 c #ebeff3",
"eT c #ebf0f2",
"e6 c #ebf0f3",
"iB c #ebf1f4",
"bi c #eca73c",
"bl c #eca83d",
"am c #eca941",
"dn c #ecae49",
"do c #ecb55b",
"#q c #ecb55e",
"ef c #ece6d9",
"qs c #ecedee",
"oc c #eceeef",
"lt c #eceef0",
"la c #eceff0",
"eO c #eceff1",
"ni c #ecf0f1",
"pV c #ecf1f3",
"mN c #ecf1f4",
"fi c #ecf1f5",
"p4 c #ecf2f5",
"c2 c #eda83b",
"cH c #eda93a",
"c3 c #edab3d",
"an c #edad48",
"cf c #edc077",
"#l c #edddc2",
"on c #edeeee",
"nL c #edeeef",
"fv c #edefef",
"i6 c #edeff0",
"kL c #edeff1",
"mM c #edf0f2",
"eU c #edf1f3",
"eV c #edf1f4",
"nE c #edf1f5",
"e5 c #edf2f4",
"nq c #edf3f6",
"cc c #eea93b",
".y c #eee4cf",
"h8 c #eeeff0",
"mb c #eeeff1",
"pm c #eef0f0",
"mS c #eef0f1",
"dT c #eef0f2",
"dW c #eef1f2",
"np c #eef1f3",
"dx c #eef2f5",
"g9 c #eef2f6",
"pB c #eef3f4",
"kM c #eef3f5",
".# c #eef3f6",
"iR c #eef4f5",
"lx c #eef4f6",
"cI c #efa938",
"bm c #efaa3b",
"ce c #efad40",
"ao c #efbc6a",
"dK c #efe3cb",
"ag c #efe3d1",
"b8 c #efece8",
"dV c #eff0f1",
"g8 c #eff0f2",
"rc c #eff1f2",
"gk c #eff1f3",
"nW c #eff2f3",
"cv c #eff2f4",
"no c #eff3f4",
"pY c #eff3f5",
".f c #eff3f6",
".g c #eff4f6",
"mY c #eff4f7",
"bn c #f0b048",
"#r c #f0c177",
"dq c #f0e2ca",
"ea c #f0f1f1",
"cz c #f0f1f2",
"et c #f0f2f2",
"ir c #f0f3f4",
"oH c #f0f3f7",
"mg c #f0f4f7",
"gJ c #f0f4f8",
".h c #f0f5f7",
".i c #f0f5f8",
"cd c #f1aa37",
"bo c #f1c886",
"cL c #f1e6d1",
"cA c #f1efeb",
"ia c #f1f1f1",
"p3 c #f1f1f3",
"hk c #f1f2f1",
"dY c #f1f2f2",
"m7 c #f1f2f3",
"fr c #f1f3f4",
"qq c #f1f3f5",
"eL c #f1f4f5",
"d0 c #f1f4f6",
"hh c #f1f4f7",
".c c #f1f5f6",
"fb c #f1f5f7",
"kz c #f1f5f8",
"jK c #f1f6f8",
".j c #f1f6f9",
"ej c #f2ece1",
"iA c #f2f3f3",
"jc c #f2f3f4",
"oo c #f2f4f4",
"mt c #f2f4f5",
"dt c #f2f4f6",
"ij c #f2f5f7",
"jC c #f2f5f8",
"eM c #f2f6f8",
".k c #f2f6f9",
".D c #f3e1c0",
"cg c #f3e4cc",
"be c #f3f1ed",
"pd c #f3f2f3",
"rd c #f3f3f4",
"gf c #f3f4f4",
"#h c #f3f4f5",
"da c #f3f5f5",
"du c #f3f5f6",
"e7 c #f3f5f7",
"cR c #f3f6f6",
"lJ c #f3f6f8",
".l c #f3f6f9",
"d9 c #f3f7f9",
"#s c #f4d7a8",
"ap c #f4d9ae",
"c6 c #f4eee2",
"hB c #f4f5f5",
"nn c #f4f6f5",
"dh c #f4f6f7",
"lB c #f4f6f9",
"lL c #f4f7f7",
"lK c #f4f7f8",
".e c #f4f7f9",
".m c #f4f8f9",
"cW c #f5f5f4",
"b6 c #f5f5f5",
"rb c #f5f5f6",
"ex c #f5f6f6",
"jT c #f5f6f7",
"qu c #f5f7f8",
"d7 c #f5f7f9",
".o c #f5f7fa",
".n c #f5f8f9",
".p c #f5f8fa",
"mT c #f5f9fa",
"bp c #f6ead7",
"#k c #f6f4ee",
"cs c #f6f6f6",
"cq c #f6f6f7",
"cu c #f6f7f6",
"cr c #f6f7f7",
"iG c #f6f7f8",
"qZ c #f6f8f8",
"cj c #f6f8f9",
"Qt c #f6f8fa",
".q c #f6f9fa",
"fI c #f6f9fb",
".x c #f7f3ed",
"dL c #f7f5ee",
"ct c #f7f7f7",
"mC c #f7f7f8",
"ha c #f7f8f7",
"gI c #f7f8f8",
"cp c #f7f8f9",
"fe c #f7f9f8",
"hw c #f7f9f9",
".s c #f7f9fa",
".r c #f7f9fb",
"nv c #f7fafb",
"af c #f8f8f7",
"bc c #f8f8f8",
"qH c #f8f8f9",
"dv c #f8f9f9",
".d c #f8f9fa",
"j2 c #f8faf9",
"q2 c #f8fafa",
"d5 c #f8fafb",
"eF c #f8fafc",
"qt c #f8fbfb",
"d3 c #f8fbfc",
"#t c #f9eede",
"dr c #f9f7f4",
"ek c #f9f8f4",
"iQ c #f9f9f9",
"qS c #f9f9fa",
"i1 c #f9fafa",
"e8 c #f9fafb",
"n4 c #f9fafc",
"qv c #f9fbfb",
"ey c #f9fbfc",
"ie c #f9fbfd",
".E c #faf1e2",
"aq c #faf4ea",
"jO c #faf9f9",
"df c #fafafa",
"lM c #fafafb",
"q0 c #fafbfa",
"d6 c #fafbfb",
".t c #fafbfc",
"pf c #fafbfd",
"dS c #fafcfc",
"ds c #fafcfd",
"ae c #fbfbfb",
"ci c #fbfbfc",
"qK c #fbfbfd",
"dR c #fbfcfc",
"gt c #fbfcfd",
"oy c #fbfdfd",
"ch c #fcfbf8",
"cM c #fcfcf9",
"nI c #fcfcfb",
".w c #fcfcfc",
"dZ c #fcfcfd",
"el c #fcfdfc",
"b7 c #fcfdfd",
"d4 c #fcfdfe",
".F c #fdfbf8",
"#u c #fdfcf9",
"nz c #fdfcfd",
"bq c #fdfdfb",
"c7 c #fdfdfc",
"#j c #fdfdfd",
"bd c #fdfdfe",
".u c #fdfefd",
"dg c #fdfefe",
"nD c #fdfeff",
"ar c #fefefd",
"#i c #fefefe",
"dw c #fefeff",
"pe c #fefffe",
"c8 c #feffff",
"eb c #fffefe",
"d8 c #fffeff",
"jd c #fffffe",
".v c #ffffff",
"Qt.#.a.a.a.a.a.a.a.a.b.c.d.e.f.#.#.#.#.#.#.g.h.i.i.h.j.k.l.l.l.l.e.e.m.n.o.p.p.p.p.p.q.q.r.r.s.t.u.v.v.v.v.v.v.w.x.y.z.A.B.C.D.E.F.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
".G.H.I.J.J.J.J.J.J.J.K.L.M.N.O.P.Q.R.S.T.U.V.W.X.X.Y.Z.0.1.2.2.3.4.5.6.7.8.9#..9###a#b#c#d#e#f#g#h#i.v.v.v.v#j#k#l#m#n#o#p#q#r#s#t#u.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
"#v#w#x#y#z#A#B#C#y#y#D#E#F#G#H#I#J#K#L#M#N#O#P#Q#R#S#T#U#V#W#X#Y#Z#0#1#2#3#4#5#6#7#8#9a.a#aaabacadae.v.v.v#iafagahaiajakalamanaoapaqar.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
"asatauavawaxayazaAaBaCaDaEaFaGaHaIaJaKaLaMaNaOaPaQaRaSaTaUaVaWaXaYaZa0a1a2a3a4a5a3a6a7a8a9b.b#babbbc.v.v.vbdbebfbgbhbibjbkblbmbnbobpbq.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
"brbsbtbubvbwbxbybzbubAbBbCbDbEbFbGbHbIacbJbKbLbLbMbNbObPbQbQbRbSbTbUbVbWbXbYbZbZbXbZbZb0b1b2b3b4b5b6.v.v.vb7b8b9c.c#cacacbcccdcecfcgch.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
"cicj.n.nckclcmcnco.ncj.sci.wcpcqcqcrcqcqcrcrcrcqcqcqcqcrcrcqcqcscrcrcrctctctctctcucrcrcqcqcvcwcxcycz.v.v.vb7cAcBcCcDcEcFcGcHcIcJcKcLcM.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
".v.v.v.vcNcOcPcQcR.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.tcScTcUcV#j.v.v#jcWcXcYcZalc0c1c2c3c4c5c6c7.v.v#i#i.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v",
".v.v.vc8c9d.cPd#da.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.vb7dbdcdddedf.v.vdgdhdidjdkdlcJdmdndodpdqdrbddsQtdtdudv#j.v#ib7ae#j#i.v#iaedv.w.v.vdw#jae.w",
".v.v.vc8c9d.cPd#da.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.vdwdxdydzdAb6.v.vcidBdCdDdEdFdGdHdIdJdKdL.w.rdMdNdOdPdQczdRdSdTdUdV.w.vb7dWdXdY#j.vdZd0d1d2",
".v.v.vc8c9d.cPd#da.v.v.vdgd3.ecjdRc8.v.v.v.v.vdwd4d5cjd6#idwc8b7.scj.tdw.vb7.sd7d5.uc8dw.vd8d9e.e#bUeaeb.v.qecedbZeeefegeheiejekeld5emeneoepeqereset.neuevewex.veyezeAeBdf.v.seCeDeE",
".v.v.vc8c9d.cPd#da.vdweFeGeHeIeJeKeL#j.vd4eMeNeOePeQeReSeTeUeVeWeXeYeZcp.ee0e1e2e3e4e5e6e7#je8e9f.f#faar#ifbfcb4fdeafefffgfhex#ib7fifjfkflfmfnfofpfqfrfsftfufv#iQtfwfxfyctdw.gfzddfA",
".v.v.vc8c9d.cPd#da.veFfBfCfDfEfFfGfHcv#ifIfJfKfLfMfNfOfPfQfRfSfTfUfVb3fWfXfYfZf0f1f2f3f4f5cqb7f6f7f8f9.w#jg.g#gagbcr.tgcgdgegf#i.rggghgigjgkf6glgmbSgngob3bIgp.w.hgqgrgsdVgtgugvgwgx",
".v.v.vc8c9d.cPgydadggzgAgBgCgDgEgFgGgHgIgJgKgLgMgNgOgPgQgRgSgTgUgVgWgXgYgZg0g1g2g3g4g5g6g7g8bdg9h.h#bbha.tePhbhchddf.thehfhgdYdghhhihjfdhk#jd5hecThlhmhnhohphqgIhrhshthuhvhwhxhyhzhA",
".v.v.vc8c9d.cPgyhB.shChDhEhFhGhHhIhJhKhL.#hMhNhOhPfPhQhRhShThUhVhWhXhYhZh0h1h2glh3h4h5h6h7h8#id9h9i.i#iaQteKibicid#jieheflifdYb7g.igihiigkd7ijikilimineUioipiqirisitiuiviwfrixiyiziA",
".v.v.vc8c9d.cPgy#hiBiCiDiEiFiGe6iHiIiJiKe6iLiMiNiOiPiQiRhHiSiTiUiVhwiWiXiYiZi0i1g9i2i3i4i5i6.veFi7i8i9j..fj#jajbjcjdeyhejeifdY.temjfjgjhjijjjkjljmjnjoiriximgsjpiojqjrjsjtjujvjwjxha",
".v.v.vc8c9d.cPgyire0jyjzjAjB.djCjDjEjFjGjH.P#KjIjJbc.vjKhHjLjMjNjOgtjPjQjRjSjT.vb7jUjVjWi5i6.vgtjXjYbFjZjHj0imj1j2.veyhejeifdY.tj3j4j5j6j7j8j7j9k.k#kaduhxiykbkckdkekfkgkhkigXddkjd6",
".v.v.vc8c9d.cPkkdW#.klkmknkokpkqkrksktkukvkwkxkygjel.vkzkAkBkCkD#ieFkEkFkGkHae.vc8cNkIkJkKkL.v#ikMhikNkOkPkQkRhv.w.veyheflifdY.tkSkTkUkVkWkXkXkXkYkZk0e8k1k2h#eDk3k4k5k6k7k8k9l.l#b7",
".v.v.vc8c9d.cPkklalblcldlelflglhliljlkimlllmlnlolp#i.vkzlqlrlslt.v.qlulvlwiF#j.v.vlxlylzlAi6.v.vlBlCimlDlElFlGfa#i.veyheflifdY.tkSb3lHlIlJ.n.nlKlJlLlM#ilJh9lNlOlPlQlRlSjrlTjrbTcz.v",
".v.v.vc8c9d.cPcQcolUlVlWlXlYlZl0l1l2l3l4l5l6l7l8l9#i.vkzm.m#mamb.v.qmcmdmemfdg.v.vmgmhmimji6.v.vd5mkmlmmmnmompdY.v.veyheflifdYdSemmqmrmsaec8#idRdhmti1#i.rmkmumvmwmxdTmymzmAgamBmC.v",
".v.v.vc8c9d.cPcQmDmEmFmGmHmIj#mJfcmKmLmMmNmOmPmQgn.v.vkzm.fOmRmS.vmTmUmVmWmXbd.v.vmYmZm0lAi6.v.vgtm1m2m3m4m5m6bc.v.veyheflhgm7b7e6m8m9n.ct.v#ilK.Mn#na.wcigckTnbncndcpemnenfmmngae.v",
".v.v.vc8c9d.cPnhninjnknlnmnnd6.t.snonpdvnqmOmPnrns.v.vkzm.ntnumS.vnvnwnxnykinz.v.v.bnAnBnCi6.v.vnDnEnFnGnHhlhvnI.v.veyheflifm7dgijnJjanKnL.weynMnNnOngd6dgnPnQnRbJfab7g9nSnTnUnV#j.v",
".v.v.vc8c9d.cPkknWnXnYnZn0ex.v.vkzn1n2n3nPmOmPmQns.v.vkzm.ntnumS.vn4n5n6n7n8ae.vdgn9o.o#i5i6.v.vc8.eoafRb#obocar.v.veyheflifm7#ieFodiloeofogkvkVibohoi.wc8.hmJojokdYdglJnJolomon#i.v",
".v.v.vc8c9d.cPgyooopoqoroshL#idSotouovowguoxmPmQns.v.vkzm.ntnumS.voy#cozoAm2oo.v.nlUoBoCoDkL.v.v.vd5oEoFmRoGiAeb.v.veyheflifm7.vdgoHoIoJoKoLeYoMjwoNhBeb.vQtoOoPoQcr.v.roRoPoSb6.v.v",
".v.v.vc8c9d.cPd#dafboTmFoUoVjHoW.PoXoYoZo0oxo1o2ns.v.vkzm.ntnumS.vc8o3o4o5o6jvo0o7o8o9p.i5kL.v.v.vgtj3p#hjeEgI.v.v.v.tpapbpcpdpe.vpfg.pgphpipjpkplpmc7.v.veypnb1poae.vcippplpqae.v.v",
".v.v.vc8o3prpsnhnn.tkWptpupvpwpxlcpypzpApBpCpDpEna.v.vkzpFpGpHmS.v.v.ppIpJpKpLpMpNpOpPpQpRkL.v.v.vbdpSpTpUhm.w.v.v.vb7pVpWpXgI.v.vdw.tpYpZp0p1p2p3.w.v.v.v#jjcj.#har.v#iexfvcqar.v.v",
".v.v.vc8p4aap5p6iG#imYlUp7p8p9q.q#qaqbgI.lj4qcqddV.v.v.eqeqfqgjc.v.vbdqhqiqjqkqlqmqnqomeqpm7jd.v.vdwqqqroGqsdRoygtgtdS.neLdhqtgtgtdRgteyquirfriGeygtgtgtgtdRd5cpd5dRgtgtqv.de8dR#j.v",
".v.v.v.v.rqwqxqyae.vdgnqnjqzqAqBqCqD#h#i.tqEqFqGqH.v.vcil5qIqJi1.v.v.vqKc9qLqMqNqOqPdPqQqRqS.v.v.v.vdvqTqUm7eOqVpZqWpZqWqWqWpZqWqWqWpZpZpZpZlIlIpZpZqWqWqWpZpZpZpZqWqWqWlIqWqWn3fr.w",
".v.v.v.vdwn4.sdR#i.v.vdg.ekvfsqXqYqZdg.vpegthwq0#j.v.vebdR.sci#i.v.v.v.vd4fbikixq1mtq2hwaedg.v.v.v.v#jdvcri6q3q4kbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbkbq5q6hqiQ",
".v.v.v.v.v.v.v.v.v.v.v.v.vdgb7b7#i.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.vbdb7dg.v.v.v.v.v.v.v.v.v.vdw#jfrq7q8q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9q9r.r#raae",
".v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.v.vdRrbrcdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVdVrdi1#i"};

static const char* const image5_data[] = { 
"70 30 1135 2",
"gt c #4f3911",
"hg c #5d4110",
"lD c #5e5646",
"l# c #625741",
"kx c #645b4a",
"e1 c #65615c",
"gu c #664915",
"iG c #665941",
"hh c #674a14",
"hW c #684911",
"fb c #685b42",
"fH c #695022",
"iH c #69655d",
"i. c #69665f",
"jb c #6a5d41",
"mf c #6a5e45",
"hA c #6b5c3f",
"ec c #6c5a35",
"jT c #6c5d40",
"j. c #6c6557",
"mg c #6c675f",
"lC c #6d5931",
"iq c #6e4f12",
"er c #6e5f3f",
"hX c #6f5015",
"fI c #6f5117",
"gs c #705526",
"h9 c #715c33",
"iz c #71654e",
"i3 c #725213",
"e0 c #725c2e",
"jO c #725d31",
"hj c #725e34",
"ed c #726241",
"fW c #72654b",
"jP c #726f6a",
"jF c #735313",
"me c #735f37",
"gQ c #73674e",
"eq c #745d2e",
"it c #745f35",
"kB c #746546",
"jg c #746f64",
"fc c #747472",
"ir c #755616",
"fK c #756034",
"gw c #756035",
"i6 c #756135",
"hB c #756d5b",
"km c #765615",
"kl c #765a27",
"jI c #766136",
"ld c #766646",
"jc c #76694c",
"mp c #76694e",
"iU c #766953",
"jf c #766d58",
"jU c #767472",
"k1 c #77591f",
"iF c #775f2d",
"mw c #776641",
"h4 c #77694b",
"gI c #776a4d",
"mR c #776f5e",
"mS c #776f5f",
"mV c #777165",
"lE c #77736e",
"la c #777571",
"k2 c #785815",
"i4 c #785817",
"ee c #786742",
"es c #786846",
"mT c #78705f",
"mU c #787160",
"kw c #795d26",
"lP c #796436",
"fl c #796438",
"f9 c #797366",
"et c #7a6947",
"je c #7a6d52",
"mW c #7a756c",
"lx c #7b5914",
"hY c #7b5a16",
"md c #7b622e",
"eD c #7b653a",
"jd c #7b6d50",
"fL c #7b7770",
"hi c #7c5b16",
"jG c #7c5b18",
"l. c #7c5f24",
"f4 c #7c6e4f",
"mQ c #7c7463",
"m7 c #7c7464",
"eb c #7d6024",
"gP c #7d632c",
"kp c #7d642f",
"eu c #7d6b44",
"nd c #7d7667",
"m6 c #7d7668",
"jE c #7e612f",
"m. c #7e6228",
"hq c #7e7051",
"is c #7f5d18",
"kn c #7f5e19",
"l9 c #7f632c",
"mc c #7f642a",
"gi c #7f7a70",
"ef c #806939",
"f8 c #80796c",
"jh c #807d77",
"mo c #816326",
"iE c #816423",
"mb c #816427",
"ju c #817a6c",
"d3 c #817c73",
"mX c #817e78",
"mh c #817f7a",
"fJ c #82601a",
"lB c #826321",
"lw c #826323",
"i9 c #826325",
"mv c #826425",
"m# c #826426",
"ma c #826526",
"i5 c #83611a",
"mn c #836627",
"#W c #837c6f",
"kS c #837e73",
"lQ c #83817b",
"mu c #846729",
"fx c #847f75",
"fm c #84817c",
"gv c #856219",
"iD c #856520",
"f3 c #856728",
"nc c #857d70",
"lp c #857f71",
"gx c #858178",
"jJ c #85827a",
"eE c #85827d",
"ly c #866319",
"h8 c #86651f",
"iC c #86661f",
"iB c #866620",
"hs c #86692c",
"dQ c #867042",
"ne c #868176",
"iy c #876720",
"mm c #876a2c",
"ev c #876c32",
"iA c #87744b",
"kq c #877b62",
"il c #877f70",
"jH c #88651a",
"hz c #88671e",
"eg c #886a27",
"i2 c #886a33",
"mx c #888681",
"dw c #89661b",
"h3 c #89681e",
"hf c #896c31",
"h5 c #89754c",
"kT c #898378",
"dx c #8a671a",
"dv c #8a671b",
"dJ c #8a671c",
"fa c #8a671d",
"hp c #8a681f",
"gE c #8a6923",
"fi c #8a6b2c",
"j# c #8a774d",
"hK c #8a806e",
"m8 c #8a857b",
"e2 c #8a8782",
"lN c #8b6515",
"c4 c #8b6516",
"c6 c #8b6615",
"h7 c #8b6715",
"dO c #8b6718",
"cS c #8b6719",
"ko c #8b671b",
"kA c #8b671c",
"dG c #8b681a",
"dI c #8b681b",
"jS c #8b681c",
"k9 c #8b681d",
"k4 c #8b691f",
"gR c #8b6d2e",
"gF c #8b8068",
"cG c #8c6617",
"ds c #8c6718",
"ej c #8c671a",
"c5 c #8c6818",
"dy c #8c6819",
"cT c #8c681a",
"du c #8c681b",
"ep c #8c681c",
"eB c #8c6919",
"dP c #8c691a",
"dH c #8c691b",
"eh c #8c691c",
"i# c #8c8474",
"eP c #8c877c",
"lA c #8d6817",
"dz c #8d6818",
"cV c #8d6819",
"ex c #8d681a",
"cR c #8d681b",
"k3 c #8d681c",
"cU c #8d6919",
"cH c #8d691a",
"dt c #8d691b",
"cW c #8d691c",
"gD c #8d6a1a",
"ei c #8d6a1b",
"ea c #8d6a1c",
"ew c #8d6a1e",
"k5 c #8d6d29",
"f5 c #8d6f30",
"e6 c #8d6f33",
"ip c #8d6f34",
"fS c #8d7137",
"fj c #8d7d5e",
"fG c #8d7f66",
"hL c #8d8577",
"m5 c #8d8677",
"h6 c #8e6918",
"eC c #8e691a",
"lI c #8e6a19",
"eZ c #8e6a1a",
"f# c #8e6a1b",
"cI c #8e6a1c",
"eo c #8e6a1d",
"f. c #8e6b1e",
"mt c #8e6f31",
"hr c #8e7b51",
"hJ c #8e7d62",
"iT c #8e7f65",
"ka c #8e887c",
"lo c #8e887d",
"ln c #8e897e",
"ky c #8e8c87",
"lO c #8f6918",
"lz c #8f6a1a",
"kv c #8f6a1b",
"hy c #8f6b1b",
"fV c #8f6b1e",
"gH c #8f6b1f",
"k8 c #8f6c1e",
"cJ c #8f6c20",
"hC c #8f702f",
"hV c #8f7235",
"g. c #8f8878",
"#X c #8f897b",
"ja c #906b1e",
"jR c #906b1f",
"cX c #906c20",
"fR c #906d23",
"gZ c #908a7c",
"fk c #917334",
"gY c #918a7b",
"eQ c #918c81",
"fh c #92763a",
"e5 c #92856b",
"fr c #928a7b",
"aW c #928b7d",
"g0 c #928b7e",
"mY c #928e88",
"dA c #93712a",
"l8 c #937945",
"a# c #938c7d",
"bC c #938c7e",
"af c #938d80",
"ia c #947534",
"ht c #947b43",
"jQ c #948056",
"jt c #948e80",
"g7 c #948e82",
"eU c #948e83",
"iI c #949390",
"k7 c #95732c",
"cK c #95742d",
"k6 c #957530",
"eY c #957634",
"mP c #958c7b",
"l2 c #958f83",
"cY c #967530",
"ml c #96783b",
"d4 c #969084",
"kC c #96938f",
"i8 c #97742f",
"k# c #979083",
"g1 c #979183",
"ae c #979184",
"eT c #979287",
"fX c #979492",
"hx c #98762f",
"lq c #989184",
"gO c #997833",
"eA c #997b3b",
"e4 c #99958c",
"dR c #99958f",
"ey c #9a8353",
"l4 c #9a958a",
"e3 c #9a9790",
"fd c #9a9995",
"cF c #9b7a36",
"dr c #9b7a37",
"iK c #9b7c3b",
"gG c #9b875d",
"e7 c #9b8860",
"#Y c #9b9485",
"e# c #9c7c37",
"dK c #9c8044",
"fg c #9c9483",
"l1 c #9c9484",
"cr c #9c9485",
"hI c #9d8c70",
"eH c #9d9484",
"fy c #9d9689",
"l3 c #9d968a",
"lM c #9e7d3b",
"dN c #9e7f3e",
"gC c #9e8043",
"ad c #9e9788",
"fE c #9e988c",
"ji c #9e9d98",
"c3 c #9f7f3e",
"j3 c #9f803f",
"ek c #9f8654",
"l5 c #9f9b90",
"le c #9f9c97",
"f2 c #a08750",
"c7 c #a08a5c",
"ih c #a08d6d",
"dW c #a09886",
"eS c #a09a8f",
"jV c #a09e99",
"ff c #a0a09e",
"ii c #a18d6c",
"jv c #a19781",
"gX c #a19987",
"fe c #a1a09e",
"ao c #a29a8a",
"hO c #a29c8f",
"mq c #a29f9a",
"kz c #a38f65",
"ku c #a48546",
"cL c #a4894e",
"ik c #a4957d",
"ac c #a49d8b",
"av c #a49d8e",
"gJ c #a4a09a",
"oy c #a58d67",
"dc c #a59d8b",
"#Z c #a59d8c",
"au c #a59d8d",
"ax c #a59d8f",
"a7 c #a59e91",
"hP c #a59f91",
"dB c #a68d5a",
"ij c #a69374",
"hM c #a69e8f",
"kU c #a69f91",
"eV c #a6a194",
"nf c #a6a49d",
"ig c #a7926f",
"fF c #a7a296",
"ix c #a88b52",
"iV c #a8987b",
"gr c #a89a80",
"gj c #a8a193",
"fT c #a8a294",
"hw c #a98d51",
"hu c #a98f57",
"fQ c #a99774",
"ag c #a9a192",
"fD c #a9a294",
"d5 c #a9a396",
"iQ c #aa9f8a",
"aX c #aaa08e",
"dd c #aaa18e",
"kR c #aaa393",
"eR c #aaa396",
"an c #aba28f",
"aw c #aba395",
"jN c #ac8f55",
"ms c #ac925c",
"fU c #ac986e",
"kb c #aca596",
"iS c #ad9b7d",
"iR c #ad9f84",
"ab c #ada391",
"jW c #adaba5",
"hv c #ae945b",
"cq c #aea693",
"ay c #aea698",
"lF c #aeaaa3",
"jX c #aeaca7",
"cZ c #af9767",
"ez c #afa38d",
"ho c #b09865",
"oz c #b09870",
"jw c #b0a48c",
"im c #b0a796",
"d2 c #b0aa9b",
"iP c #b1a793",
"a8 c #b1aa9b",
"jY c #b1afab",
"aa c #b2a996",
"if c #b39b73",
"lc c #b39e74",
"hH c #b3a895",
"mM c #b3ada0",
"nZ c #b49a71",
"oW c #b4a385",
"bD c #b4ab98",
"h2 c #b59d6a",
"dX c #b5ab97",
"l0 c #b5ad9c",
"iJ c #b5ad9d",
"fq c #b6ac97",
"g# c #b6ac98",
"g2 c #b6ad9d",
"g6 c #b6af9f",
"lr c #b6afa0",
"mN c #b6afa3",
"oX c #b7a685",
"k0 c #b7a787",
"j6 c #b7a88c",
"g8 c #b7af9f",
"bN c #b7b0a1",
"iO c #b8a484",
"ph c #b8a584",
"jo c #b8a687",
"n0 c #b8a98c",
"bO c #b8b0a1",
"lb c #b8b5af",
"m9 c #b8b6b0",
"e9 c #b9a47a",
"fz c #b9b1a1",
"ox c #baa37f",
"oV c #baad94",
"at c #bab09d",
"d6 c #bab2a3",
"dl c #bab3a2",
"ny c #bab5a9",
"nz c #bab5aa",
"fM c #bab5ab",
"jZ c #bab8b3",
"kM c #bba582",
"lv c #bbaa87",
"pg c #bbaa8e",
"l7 c #bbad90",
"kN c #bbb098",
"ap c #bbb09c",
"jp c #bbb29e",
"gh c #bbb4a3",
"nA c #bbb6ab",
"nB c #bbb7ac",
"lH c #bca77b",
"lk c #bca888",
"oY c #bca989",
"jx c #bcac8e",
"cs c #bcb29d",
"#0 c #bcb39f",
"m4 c #bcb4a5",
"nC c #bcb8b0",
"j5 c #bda37b",
"kr c #bdb094",
"a. c #bdb39d",
"l6 c #bdb7aa",
"nx c #bdb8ab",
"bU c #bea879",
"bW c #bea97c",
"bX c #bea97d",
"bY c #beaa7d",
"lS c #bead91",
"eI c #beb39d",
"fC c #beb7a8",
"bV c #bfa97b",
"nj c #bfaa87",
"bZ c #bfab7e",
"en c #bfab7f",
"pf c #bfb198",
"iW c #bfb299",
"nN c #bfbdb7",
"dF c #c0ac7f",
"cQ c #c0ac80",
"b8 c #c0ad82",
"oZ c #c0ad8b",
"c. c #c0ae83",
"mB c #c0b299",
"aY c #c0b5a0",
"f7 c #c0b69e",
"gW c #c0b69f",
"kc c #c0b8a7",
"mL c #c0bbab",
"nM c #c0beb8",
"kt c #c1ae81",
"b9 c #c1ae83",
"b7 c #c1ae84",
"c# c #c1ae85",
"ca c #c1af85",
"jy c #c1b08f",
"eX c #c1b394",
"j7 c #c1b9a5",
"a6 c #c1b9a7",
"hQ c #c1baa8",
"nD c #c1beb6",
"cl c #c2af86",
"pi c #c2b294",
"kk c #c2b497",
"kf c #c2b69c",
"nk c #c2b6a0",
"de c #c2b8a1",
"lZ c #c2bba9",
"ck c #c3b087",
"cj c #c3b088",
"b0 c #c3b188",
"ke c #c3b69c",
"pe c #c3b69e",
"bE c #c3b9a3",
"aq c #c3b9a4",
"bB c #c3baa2",
"kV c #c3bcaa",
"nU c #c3c1bf",
"cM c #c4b695",
"kW c #c4bba6",
"dL c #c4bdac",
"mK c #c4bdad",
"mA c #c5b290",
"lT c #c5baa5",
"kX c #c5bba6",
"g3 c #c5bdaa",
"gk c #c5bdab",
"mJ c #c5bdac",
"fP c #c5c0b6",
"bT c #c6b388",
"cb c #c6b48e",
"oA c #c6b491",
"jq c #c6bda8",
"ah c #c6bfac",
"cz c #c6bfae",
"fA c #c6bfaf",
"el c #c6c0b0",
"c8 c #c6c3be",
"o0 c #c7b695",
"he c #c7ba9c",
"dY c #c7bca6",
"eG c #c7bda5",
"nL c #c7c5c0",
"nT c #c7c5c2",
"mk c #c8b793",
"pd c #c8bda6",
"kO c #c8bea8",
"hN c #c8c0ae",
"mO c #c8c0af",
"dk c #c8c0b0",
"fB c #c8c0b1",
"mZ c #c8c4bd",
"nO c #c8c5c0",
"nV c #c8c8c5",
"#. c #c9bea7",
"as c #c9bea8",
".9 c #c9bfa7",
"nw c #c9c3b7",
"eW c #c9c3b8",
"mC c #cac0aa",
"a5 c #cac0ae",
"dm c #cac2b0",
"e8 c #cac7c1",
"j0 c #cac8c4",
"jj c #cac9c6",
"jn c #cbb690",
"lR c #cbb996",
"ct c #cbc0a9",
"pc c #cbc0ab",
"py c #cbc1ac",
"fw c #cbc2b1",
"gq c #cbc4b5",
"nE c #cbc7be",
"b1 c #ccbc99",
"gN c #ccbc9a",
"ks c #ccbe9d",
"#V c #ccc1aa",
"mI c #ccc1ad",
"px c #ccc2ad",
"js c #ccc2ae",
"lY c #ccc2af",
"cA c #ccc3b1",
"dC c #cdc3b1",
"ls c #cdc5b6",
"ci c #cebd9d",
"n1 c #cec0a7",
"fs c #cec2ab",
"ar c #cec2ac",
"kd c #cec3b0",
"a4 c #cec4b0",
"d7 c #cec6b6",
"f1 c #cec9bf",
"nY c #cfbb96",
"cc c #cfc0a2",
"jD c #cfc1a5",
"eJ c #cfc2ac",
"hk c #cfc9bf",
"gy c #cfcabf",
"hZ c #cfcac0",
"o1 c #d0c0a0",
"pb c #d0c5b0",
"jr c #d0c5b1",
"nl c #d0c6af",
"g9 c #d0c7b5",
"az c #d0c8b6",
"jK c #d0ccc3",
"gS c #d0d0ce",
"ll c #d1c5af",
"pw c #d1c7b1",
"#1 c #d1c8b4",
"gp c #d1cab9",
"fY c #d1cec9",
"fn c #d1d0cd",
"eF c #d1d0ce",
"ie c #d2c1a6",
"oU c #d2c5ac",
"hG c #d2c5ad",
"lj c #d3c1a0",
"eO c #d3cbb8",
"kY c #d3cbba",
"dM c #d3cbbd",
"jk c #d3ccbe",
"fN c #d3cec2",
"my c #d3d2cf",
"i1 c #d4c5a7",
"jz c #d4c7ae",
"ga c #d4c9b1",
"c0 c #d4cbba",
"fO c #d4cfc5",
"hU c #d5c5a7",
"df c #d5c9b0",
"a3 c #d5cab4",
"pv c #d5cbb6",
"pz c #d5cbb7",
"gB c #d5cdbc",
"nt c #d5cebe",
"iN c #d6c4a4",
"aV c #d6cab1",
"aZ c #d6cbb2",
"cm c #d6cbb4",
"mH c #d6cbb6",
"ai c #d6cdb8",
"cB c #d6cdba",
"kD c #d6d4cd",
"io c #d7c6a8",
"dq c #d7c7a7",
"o2 c #d7c7aa",
"i7 c #d7c7ab",
"cE c #d7c8a7",
"e. c #d7c8a8",
"oB c #d7c8ab",
"b2 c #d7cab0",
"dV c #d7cbb0",
"iX c #d7cbb4",
"kg c #d7cfbd",
"nb c #d7d2c6",
"nS c #d7d7d5",
"ni c #d8c7a7",
"pa c #d8cdb5",
"g4 c #d8ceba",
"a9 c #d8d0bd",
"nF c #d8d3c9",
"mi c #d8d4cb",
"dS c #d8d8d4",
"lU c #d9cdb6",
"pj c #d9cfb9",
"hR c #d9d0bd",
"np c #d9d1bf",
"go c #d9d1c0",
"nu c #d9d2c2",
"nW c #d9d7d3",
"ow c #dac9aa",
"j8 c #dacfb9",
"pu c #dad0b9",
"bM c #dad1bc",
"nv c #dad3c3",
"## c #dbcfb6",
"no c #dbd0bd",
"lX c #dbd1bd",
"ns c #dbd3c0",
"dZ c #dcd0b6",
"b6 c #dcd0b7",
"a2 c #dcd1b9",
"kL c #ddccaf",
"id c #ddd1b8",
"cd c #ddd2c0",
"lm c #ddd3be",
"nq c #ddd5c2",
"nP c #dddbd5",
"hF c #ded2b6",
"bF c #ded3b9",
"d8 c #ded7c5",
"ng c #deddd9",
"oC c #dfd3b9",
"mD c #dfd4bc",
"pt c #dfd4bd",
"bL c #dfd5c0",
"gl c #dfd5c1",
"d1 c #dfd6c1",
"gn c #dfd6c2",
"nr c #dfd7c5",
"lG c #dfdcd5",
"fZ c #dfdeda",
"j1 c #dfdedb",
"o3 c #e0d3ba",
"ic c #e0d4b9",
"#2 c #e0d6c0",
"n7 c #e0d6c2",
"pK c #e0d6c3",
"mG c #e0d7c2",
"lt c #e0d8c8",
"nK c #e0ddd6",
"f0 c #e0dfdd",
"gV c #e1d4b7",
"jM c #e1d4bd",
"a1 c #e1d5bd",
"bK c #e1d7c0",
"oH c #e1d7c3",
"cN c #e1dacd",
"mz c #e2d4b6",
"eK c #e2d5bb",
"db c #e2d6b8",
"gb c #e2d6bd",
"kP c #e2d7c0",
"k. c #e2d8c1",
"n8 c #e2d8c6",
"in c #e2d9c5",
"pL c #e2d9c6",
".8 c #e3d6bb",
"p# c #e3d6bc",
"a0 c #e3d7be",
"nn c #e3d8c0",
"pA c #e3d9c5",
"pJ c #e3dac5",
"gm c #e3dac7",
"m0 c #e3dfd5",
"j4 c #e4d5b8",
"ib c #e4d7bc",
"am c #e4d8bd",
"n6 c #e4d9c1",
"oG c #e4dac3",
"g5 c #e4dac4",
"pk c #e4dbc7",
"dj c #e4dcc6",
"gK c #e4e0d8",
"oT c #e5d7bc",
"#a c #e5d9be",
"n2 c #e5d9bf",
"bS c #e5d9c0",
"bJ c #e5dac2",
"o6 c #e5dac7",
"aj c #e5dbc5",
"mF c #e5dbc6",
"bP c #e5dbc7",
"dn c #e5dbc8",
"cy c #e5dcc5",
"nG c #e5e1d6",
"hD c #e6d9b9",
".5 c #e6d9ba",
"hE c #e6d9bb",
"#S c #e6dab9",
"aT c #e6daba",
".6 c #e6dabc",
"#T c #e6dabd",
"ps c #e6dabf",
"oD c #e6dbc2",
"iY c #e6dbc4",
"oI c #e6dbc7",
"pI c #e6dcc7",
"b3 c #e6decd",
"hd c #e6dfcf",
"m3 c #e6dfd4",
"lJ c #e6e2db",
"lf c #e6e3dc",
"gU c #e7d9b9",
"f6 c #e7d9ba",
".4 c #e7d9bd",
"pp c #e7dabd",
"iL c #e7dabe",
"p. c #e7dabf",
"da c #e7dbba",
"pq c #e7dbbe",
"pC c #e7dcbd",
".# c #e7dcbe",
"j9 c #e7dcc4",
"lW c #e7ddc6",
"gg c #e7ddc7",
"cC c #e7ddc9",
"o5 c #e7dec9",
"jA c #e7decb",
"kZ c #e7dfcd",
"c2 c #e7e0d1",
"fp c #e8daba",
"#R c #e8dabe",
"dU c #e8dbbb",
".7 c #e8dbbd",
"d# c #e8dbbe",
"iM c #e8dbbf",
"jl c #e8dbc0",
"cp c #e8dcbd",
".a c #e8dcc0",
".b c #e8dcc1",
"bI c #e8ddc4",
"o4 c #e8ddc8",
"b. c #e8dfca",
"o7 c #e8dfcb",
"hc c #e8dfcd",
"kh c #e8e0cd",
"lL c #e8e0d1",
"li c #e8e1d2",
"kE c #e8e6e0",
"fo c #e9dbbe",
"jm c #e9dcc0",
"pr c #e9dcc1",
"Qt c #e9dcc2",
"aU c #e9ddbe",
"bA c #e9ddbf",
"#9 c #e9ddc0",
"pD c #e9ddc2",
"bH c #e9ddc4",
"d0 c #e9dec1",
"gc c #e9dec4",
"#3 c #e9dec6",
"aA c #e9dec9",
"fv c #e9dfc9",
"pl c #e9dfcc",
"pM c #e9dfcd",
"mE c #e9e0c8",
"h. c #e9e0cc",
"kK c #e9e1d2",
"o# c #e9e2d0",
"oe c #e9e2d3",
"of c #e9e3d5",
"kF c #e9e7e2",
"#U c #eaddc0",
".c c #eaddc2",
"bG c #eadec3",
"cu c #eadec4",
"cv c #eadec5",
"cw c #eadec6",
"#b c #eadfc4",
"dg c #eadfc7",
"fu c #eadfc8",
"lV c #eae0c8",
"dh c #eae0c9",
"pH c #eae0ca",
"n9 c #eae2ce",
"hb c #eae2d0",
"d9 c #eae2d1",
"od c #eae3d3",
"og c #eae4d6",
"oh c #eae4d7",
"oi c #eae5d7",
"n. c #eae8e2",
"eL c #ebdec3",
"pE c #ebdec5",
"ft c #ebdfc6",
"n3 c #ebe0c7",
"cx c #ebe0c8",
"eM c #ebe0c9",
".i c #ebe0ca",
"oF c #ebe0cb",
"pG c #ebe1c9",
"#e c #ebe1ca",
".j c #ebe1cc",
"iw c #ebe1d0",
"#f c #ebe2ca",
"eN c #ebe2cb",
"pB c #ebe2ce",
".k c #ebe3ce",
"o. c #ebe3d1",
"oc c #ebe4d4",
"hn c #ebe4d6",
"em c #ebe7e0",
"dD c #ebe8e0",
"kG c #ebeae5",
"c9 c #ebeae8",
".d c #ecdfc4",
"#c c #ece0c8",
"n5 c #ece0cb",
"pF c #ece1c6",
".g c #ece1c7",
"#d c #ece1c8",
"kQ c #ece1c9",
".h c #ece1ca",
"#g c #ece2ca",
"#4 c #ece2cb",
"di c #ece3cb",
"iZ c #ece3ce",
"oa c #ece3d1",
".l c #ece4ce",
"pm c #ece4cf",
"lu c #ece4d4",
"mr c #ece6d8",
"oj c #ece6db",
"gd c #ede1c8",
"nm c #ede1cd",
".f c #ede2c8",
"ge c #ede2ca",
"n4 c #ede2cb",
"ak c #ede2cd",
"gf c #ede3cb",
"oE c #ede3cd",
".m c #ede4ce",
"ha c #ede4d0",
"bQ c #ede5d0",
"jB c #ede5d2",
"ob c #ede5d3",
".e c #eee1c8",
"#h c #eee2cc",
"#i c #eee2ce",
"#j c #eee3ce",
"#5 c #eee4cc",
"#k c #eee5cf",
"h# c #eee5d0",
"b# c #eee5d1",
"#l c #eee6d1",
"#m c #eee6d2",
"i0 c #eee6d3",
"ok c #eee8de",
"hl c #eee9dc",
"ce c #eeeae2",
"#7 c #efe2cf",
"#6 c #efe3cd",
".n c #efe4ce",
".o c #efe4d1",
".p c #efe6d1",
"pN c #efe6d2",
"#n c #efe6d5",
"oJ c #efe6d6",
".q c #efe7d2",
"hS c #efe7d3",
"hT c #efe7d5",
"ki c #efe7d6",
".r c #efe8d4",
".s c #efe8d5",
"iu c #efe9db",
"gz c #efe9dc",
"nH c #efebe0",
"gA c #efebe1",
"kH c #efeee9",
"#o c #f0e7d6",
"dp c #f0e8d4",
".t c #f0e8d8",
"h1 c #f0e8d9",
"be c #f0e9db",
"bf c #f0eadd",
"bg c #f0ebdd",
"oo c #f0ede6",
"cn c #f0efec",
"nR c #f0f0ee",
"ba c #f1e8d6",
"bb c #f1e8d7",
"bc c #f1e8d8",
"bd c #f1e8d9",
"do c #f1e9d4",
"bR c #f1e9d8",
".u c #f1e9d9",
"ol c #f1ebdf",
"jL c #f1ece1",
"bh c #f1edde",
"mj c #f1ede4",
"op c #f1eee7",
"nX c #f1f0ed",
"oK c #f2e8d6",
"#p c #f2e9d6",
"o8 c #f2e9d8",
"#q c #f2ead8",
"#r c #f2eada",
"#s c #f2eadb",
"cD c #f2ebdb",
"#t c #f2ebdd",
"b4 c #f2ece2",
"#u c #f2edde",
"bi c #f2ede1",
"gM c #f2ede2",
"pn c #f2eede",
"cO c #f2eee4",
"oL c #f3ead8",
".v c #f3ead9",
"aB c #f3ebd9",
".w c #f3ebda",
"aC c #f3ebdb",
".x c #f3ebdc",
"pO c #f3ebdd",
"jC c #f3ecdb",
"aD c #f3ecde",
".y c #f3ecdf",
"#v c #f3eede",
"#w c #f3eedf",
".z c #f3eee0",
"#x c #f3eee2",
"m1 c #f3eee4",
"pP c #f3efe0",
"nJ c #f3efe7",
"nQ c #f3f1ea",
"c1 c #f3f4f2",
"kj c #f4ecdb",
"po c #f4eee2",
"aE c #f4efdf",
".A c #f4efe0",
".B c #f4efe1",
"oM c #f4efe2",
"aF c #f4efe3",
".C c #f4efe4",
"ch c #f4efe5",
"on c #f4f0e8",
"kI c #f4f3ef",
"j2 c #f4f5f3",
"#y c #f5eee2",
"h0 c #f5efe2",
"om c #f5efe3",
"pQ c #f5efe4",
"nI c #f5f1e5",
"gL c #f5f2e9",
"ot c #f5f4f1",
"ou c #f5f5f2",
"#z c #f6efe3",
".D c #f6efe4",
"#A c #f6f0e5",
"al c #f6f0e6",
"hm c #f6f1e5",
"#B c #f6f1e6",
"#C c #f6f2e6",
"b5 c #f6f2e7",
"#D c #f6f2e8",
"#E c #f6f2ea",
"oq c #f6f2eb",
"bn c #f6f3ea",
"oN c #f7f0e4",
".E c #f7f0e5",
"bl c #f7f2e7",
".F c #f7f2e8",
"dE c #f7f2ea",
".G c #f7f3e8",
"bm c #f7f3e9",
".H c #f7f3ea",
".I c #f7f3eb",
"cP c #f7f4ea",
"bo c #f7f4eb",
"#H c #f7f5ed",
"na c #f7f5f0",
"os c #f7f7f5",
"iv c #f8f1e7",
"bj c #f8f2e8",
"bk c #f8f3e9",
"#F c #f8f3ea",
"m2 c #f8f3eb",
"#G c #f8f4ea",
"aG c #f8f4eb",
"bp c #f8f4ec",
"bq c #f8f4ef",
".J c #f8f5eb",
"#8 c #f8f5ed",
"br c #f8f5f0",
".K c #f8f6ec",
"pR c #f8f6ee",
"bs c #f8f6f0",
".L c #f8f7ef",
"aH c #f9f5eb",
"o9 c #f9f6ee",
"aI c #f9f6ef",
"or c #f9f6f1",
".M c #f9f7ef",
"ov c #f9f7f5",
"bt c #f9f8f2",
"cf c #f9f8f4",
"kJ c #f9f8f5",
"gT c #f9faf9",
"#I c #faf6ee",
"#J c #faf6ef",
"#K c #faf6f2",
".N c #faf7ef",
".O c #faf7f0",
".P c #faf7f3",
"lK c #faf8f1",
"#L c #faf8f2",
".Q c #faf8f3",
"#M c #faf9f2",
".R c #faf9f3",
".S c #fafaf3",
"#N c #fafaf4",
".T c #fafaf5",
".U c #fafaf7",
"co c #fafaf8",
"dT c #fafaf9",
"aO c #fafbf5",
"oP c #fbf6ef",
"oO c #fbf7ef",
"aJ c #fbf7f0",
"aK c #fbf7f1",
"aL c #fbf7f4",
"lg c #fbf8f2",
"aM c #fbf9f3",
"aN c #fbfaf3",
"#O c #fbfaf6",
".V c #fbfaf7",
"lh c #fbfbf5",
"nh c #fbfbfa",
"bu c #fbfcf7",
"bz c #fcfaf7",
"pS c #fcfaf8",
"bx c #fcfbf7",
"n# c #fcfbf8",
"by c #fdfaf7",
".W c #fdfaf8",
"cg c #fdfcfa",
"bw c #fdfdf9",
"bv c #fdfdfb",
"d. c #fdfdfc",
"#P c #fefbfa",
".X c #fefcfa",
".Y c #fefdfa",
".Z c #fefefa",
".0 c #fefefb",
".1 c #fefefc",
"#Q c #fefefd",
".2 c #fefefe",
"aP c #fffcfb",
"oQ c #fffdfb",
"aQ c #fffdfc",
"oR c #fffefb",
"oS c #fffefc",
"aS c #fffffb",
"aR c #fffffc",
".3 c #ffffff",
"Qt.#.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.s.t.u.v.w.x.x.x.y.z.A.B.C.D.E.F.G.G.G.H.I.J.K.L.M.N.N.O.P.Q.R.S.T.U.V.V.V.W.X.Y.Y.Z.Z.0.1.2.2.3.3",
".4.5.6.7.8.9#.###a#b#c#d#e#f#g#h#i#j#k#l#m#m#n#o#p#q#r#r#s#t#u#v#w#x#y#z#A#B#C#C#D#E#F#G#H#I#I#I#J#K#L#L#M#N#O#O#O#O.W#P.X.Y.Z.Z.0#Q.2.2.3.3",
"#R#S#T#U#V#W#X#Y#Z#0#1#2#3#4#5#6#7#j#k#l#m#m#n#o#p#q#r#r#s#t#u#v#w#x#y#z#A#B#C#C#D#E#F#G#8#I#I#I#J#K#L#L#M#N#O#O#O#O.W#P.X.Y.Z.Z.0#Q.2.2.3.3",
"#R#S#T#9a.a#aaabacadaeafagahaiajak#j#k#l#m#m#n#o#p#q#r#r#s#t#u#v#w#x#y#zal#B#C#C#D#E#F#G#8#I#I#I#J#K#L#L#M#N#O#O#O#O.W#P.X.Y.Z.Z.0#Q.2.2.3.3",
"#R#S.#amanaoapaqarasatauavawaxayazaA.m#l#m#m#n#o#paBaCaC.xaD#waE.AaF#z#zal#B#C#C#D#EaGaHaI.NaJaJaKaLaMaMaNaO#O#O#O#O.WaPaQaQaRaS.0#Q.2.2.3.3",
"#RaTaUaVaWaXaYaZa0a1a2a3a4a5a6a7a8a9b.b##m#m#n#obabbbcbcbdbebfbgbhbi#Abjbkbl#C#C#D#Ebmbmbnbobpbpbpbqbrbsbtbubvbwbx.Vby.Wbzbzbxbw.0.1.2.2.3.3",
"#RaTbAbBbCbDbEbFbGbHbIbJbKbLbMbNbOazbPbQ#m#m#nbRbSbTbUbVbVbWbXbYbZb0b1b2b3b4b5blbm.Ib6b7b8b9c.c.b9b7c#cacbcccdcecfcgchcicjckclcmcnco.2.2.3.3",
"#RaTcpcqcrcsctamcucvcwcx#e#4cyczcAcBcC#m#m#m#ncDcEcFcGcHcHcHcHcHcHcIcJcKcLcMcNcOcP.IcQcRcScTcTcUcUcUcVcVcWcXcYcZc0c1c2c3c4c5c6c7c8c9d..2.3.3",
"d#dadbdcdddedf.abGcvcwdgdhdidjdkdldmdn#ldodp#o.xdqdrdsdtdtdudvdwdxdydzdydydAdBdCdDdEdFcWdGdHdtdIdJdwdwdxc5dyc5dtdKdLdMdNdOdPdzdQdRdSdT.3.3.3",
"d#dUdVdWdXdYdZd0bGcvcwdgdhdid1d2d3d4d5d6d7d8d9cDe.e#cTeacIebecedeeefegeheidyejekelemeneodHeaepeqereseteueveweadPexeyezeAeBeaeCeDeEeFdT.3.3.3",
"d#dUeGeHeIeJeKeLbGcvcwdgeMeNeOePeQeReSeTeUeQeVeWeXeYcTcIeZe0e1e2e3e4e5e6eCeacHepe7e8e9f.dHf#fafbfcfdfefffgfhcHeacHfifjfkdteaeCflfmfndT.3.3.3",
"fofpfqfrfqfsam.d.dftcwfu#efvfwfxfyfzfAfBfCfDfEfFfGfHfIfJexfKfLfMfNfOfPfQfRcHeadyfSfTfUfVdtf#fafWfXfYfZf0f1f2c5eaf#f3f4f5f#eaeCflfmfndT.3.3.3",
"fof6f7f8f9g.g#gagbgcgdgegfggghgigjgkglgmgngogpgqgrgsgtgugvgwgxgygz#xgAgBgCcVeagDgEgFgGgHeif#dJgIgJgKgLgMgNgOdyeaf#gPgQgReieaeCflfmgSgT.3.3.3",
"#RgUgVgWgXgYgZg0g1aug2g3g4g5g6g7g8g9h.h#hahbhchdhehfhghhhihjgxhkhlaFhmhnhocWdHf#hphqhrgHeieaehhshthuhvhwhxf#eahyhzhAhBhCf#eaeCflfmeFdT.3.3.3",
"#RhDhEhFhGdYaYhHhIhJhKhLhMhNhOhPhQhR.l#l#mhShT#rhUhVhWhXhYhjgxhZgzh0.Eh1h2cWdHf#h3h4h5gHeieaeaeZh6h7c6c6c5f#eih8h9i.i#iacHeaeCflfmfndT.3.3.3",
"#RhD.6#TibicidieifigihiiijikilimhNin.p.q#m#m#ncDioipiqirisitgxhZiuh0iviwixcTeaf#iyiziAfVeieaeah3iBiCh8h8iDiEiFiGiHiIiJiKdyeaeCflfmgSgT.3.3.3",
"#R.5#TiLiLiM#9iNiOiPiQiRiSiTiUiViWiXiYiZ#mi0#naCi1i2i3i4i5i6gxhZgz#Aivi7i8dyeaf#i9j.j#jaeicIehe0jbjcjdjdjejfjgjhjijjjkdNdyeaeCflfmgSgT.3.3.3",
"#RhD#TiLiLjljmjnjojpjqjrjsjtjujvjwjxjyjzjAjB#njCjDjEjFjGjHjIjJjKjL#CjMjNcVeacIdujOjPjQjReif#jSjTjUjVjWjWjXjYjZj0j1j2hdj3c5eaeCflfmfndT.3.3.3",
"#RhD#TiLiLjmj4j5j6j7j8j9k.k#kakbkckdkekfkgkhkikjkkklkmknkokpkqkrksktkufVeicIkvkwkxkykzjRdtf#kAkBkCkDkEkFkFkGkHkIkJ.1kKj3dseaeCflfmgSgT.3.3.3",
"#RhD#TiLiL#9kLkMkNkOkPkQdhkRkSkTkUkVkWkXkYkZbcaCk0k1k2fJk3k4k5k6k7k8c5cHf#k9l.l#lalblcfVdteikoldlelflgaMaNlhbxbxbx.1lij3dseaeCflfmgSgT.3.3.3",
"#RhD#TiLiLjmljlkaYllj9cx#elmg8k#lnlolplqlrlsltlulvlwlxlyeClzh6lAlAh6eCdIlBlClDlElFlGlHcVc5lIejldlelJlK#L#M#N#O#O#O.1lLlMlNeZlOlPlQfndT.3.3.3",
"#R#S#TiLiM.4lRlSlTlU#3lVeMlWlXlYlZl0l1l2l3l4l5l6l7l8l9m.m#mamam#mbmcmdmemfmgmhlFmimjmkmlmmmnmompmqlflK#L#M#N#O#O#ObvmrmsmtmumvmwmxmydT.3.3.3",
"#R#S#TiLiMmzmAmBmCmDcwlVdh#emEmFmGmHmImJmKmLmMmNmOcsmPmQmRmSmSmTmUmVmWmXmYlFmZm0m1m2m3m4m5m6m7m8m9n.lK#L#M#N#O#O#On#nanba7ncndnenfngnh.3.3.3",
"#R#S#TiLiMninjnknlbSftfudh#f#4#hnmnnnonpnqnrnsntnunvnwnxnynynznAnBnCnDnEnFnGnHnI#D.HnJnKnLnMnNnOnPnQlK#L#M#N#O#O#O.VbznRnSnTnUnVnWnXd..2.3.3",
"#R#S#TiLiMnYnZn0n1n2n3eM#eeNn4#hn5n6n7n8n9jBo.o#oaobocodoeofogohoiojokolom#Bbl#C#D#E.Honooooooopoqor#L#L#M#N#O#O#O.VbzbzosotkIouovcg.2.2.3.3",
"#R#S#TiLiMowoxoyozoAoBoCoDeMoE.noFoGoHoIhai0oJbboKoLaCaCaCaD#waE.AoM#zoN#B#B#C#C#D#E.HaG#8.NoOoOoP#K#L#L#M#N#O#O#O#O.W#PoQoQaSoRoS#Q.2.2.3.3",
"#R#S#TiLiLoToUoVoWoXoYoZo0o1o2o3o4o5o6o7b#i0#n#obao8#r#r#s#t#u#v#w#x#y#zal#B#C#C#D#E.H#G#8o9#I#I#J#K#L#L#M#N#O#O#O#O.W#P.X.Y.Z.Z.0#Q.2.2.3.3",
"#R#S#TiLiLp.p#papbpcpdpepfpgphpipjpkplpm#mi0#n#o#p#q#r#r#s#t#upn#wpo#y#zal#B#C#C#D#E.H#G#8o9#I#I#J#K#L#L#M#N#O#O#O#O.W#P.X.Y.Z.Z.0#Q.2.2.3.3",
".4#S.6pppppqprjlpsa0ptpupvpwpxpypzpApB#l#mi0#n#o#p#q#r#r#s#t#upn#wpo#y#zal#B#C#C#D#E.H#G#8o9#I#I#J#K#L#L#M#N#O#O#O#O.W#P.X.Y.Z.Z.0#Q.2.2.3.3",
"QtpC.aprprpDpEftpFpG.ipHaApIpJpKpLpMpN.r.s.s.t.u.vaC.x.xpO.y.zpP.BpQ.D.E.F.F.G.G.H.IaG.JpR.M.N.N.O.P.Q.Q.R.T.VbzbzpS.W#P.X.X.Z.Z.0#Q.2.2.3.3"};

static const unsigned char const image6_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1e,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x32, 0xdb, 0x0e, 0xd8, 0x00, 0x00, 0x02,
    0x6e, 0x49, 0x44, 0x41, 0x54, 0x48, 0x89, 0x95, 0x95, 0x6b, 0x95, 0xdb,
    0x30, 0x10, 0x46, 0xaf, 0x73, 0x96, 0x80, 0x16, 0x82, 0x02, 0xc1, 0x86,
    0x90, 0x42, 0x68, 0x20, 0xb4, 0x10, 0x12, 0x08, 0x31, 0x84, 0x2c, 0x84,
    0x14, 0x42, 0x0d, 0xc1, 0x82, 0xe0, 0x81, 0x10, 0x41, 0x50, 0x7f, 0x44,
    0xe3, 0x8c, 0xe5, 0xd7, 0x76, 0xce, 0xf1, 0xb1, 0x9c, 0x48, 0x73, 0xf5,
    0xcd, 0x43, 0xaa, 0xf8, 0x3f, 0x4b, 0x2b, 0xbf, 0x57, 0x7b, 0x0b, 0x77,
    0x27, 0x58, 0xe7, 0x7d, 0xdf, 0x03, 0x20, 0x22, 0x88, 0x08, 0xde, 0x7b,
    0xbc, 0xf7, 0x34, 0x4d, 0xb3, 0xeb, 0x6b, 0xeb, 0xcf, 0x64, 0x9d, 0xd7,
    0x75, 0x4d, 0xdb, 0xb6, 0x00, 0x84, 0x10, 0xa8, 0xeb, 0x1a, 0xe0, 0xdb,
    0xb0, 0x8f, 0x2d, 0x80, 0xf7, 0x1e, 0xe7, 0x1c, 0x6d, 0xdb, 0xd2, 0x75,
    0x1d, 0x21, 0x04, 0x44, 0x84, 0xcb, 0xe5, 0x32, 0x4e, 0x56, 0x65, 0x7b,
    0x56, 0xee, 0x20, 0x59, 0x40, 0x08, 0x61, 0x02, 0x78, 0x3c, 0x1e, 0xc4,
    0x18, 0x27, 0x00, 0x11, 0x19, 0x95, 0x6d, 0xa9, 0xb2, 0x8a, 0xd2, 0x30,
    0x0c, 0x38, 0xe7, 0x10, 0x91, 0x19, 0xc0, 0x39, 0x47, 0x8c, 0x71, 0x7c,
    0x5b, 0x88, 0xbe, 0xb7, 0x4c, 0x41, 0x69, 0x18, 0x06, 0xbc, 0xf7, 0xb4,
    0x6d, 0x3b, 0x03, 0xa8, 0x29, 0x44, 0xcd, 0x86, 0x4c, 0x44, 0xe8, 0xfb,
    0x9e, 0xa6, 0x69, 0xd2, 0x92, 0xaa, 0x8f, 0x25, 0xc8, 0xed, 0x76, 0xc3,
    0x39, 0x37, 0x42, 0xd4, 0x79, 0x8c, 0x71, 0xa2, 0xca, 0xaa, 0xd9, 0xcb,
    0x53, 0x85, 0xc9, 0x8b, 0xdd, 0xb9, 0xb5, 0xd2, 0xb9, 0x7e, 0xdb, 0xd0,
    0x89, 0x08, 0x72, 0xbd, 0xf2, 0xf5, 0xf6, 0x3b, 0xb1, 0x83, 0x75, 0x6c,
    0x55, 0x94, 0x90, 0x72, 0x03, 0x36, 0x2f, 0x0a, 0x71, 0x40, 0xae, 0xc7,
    0x59, 0x63, 0x1f, 0x80, 0xea, 0x78, 0x3c, 0x8e, 0x61, 0xb1, 0x80, 0x2d,
    0xb3, 0x45, 0xa3, 0x10, 0x80, 0x7a, 0x65, 0xfe, 0x58, 0x75, 0x76, 0xe7,
    0xa5, 0x4a, 0x1b, 0xba, 0x18, 0x23, 0x21, 0x04, 0x00, 0xba, 0xf3, 0x19,
    0x67, 0x9c, 0xab, 0xc6, 0x07, 0x70, 0x7e, 0xa9, 0x1a, 0x43, 0xa8, 0xa0,
    0xaa, 0x69, 0x9a, 0x34, 0x0c, 0xc3, 0x0c, 0x64, 0x37, 0xa2, 0xb9, 0xb0,
    0x00, 0x9d, 0x15, 0xcd, 0x7b, 0xba, 0xb2, 0x50, 0x64, 0xad, 0xac, 0x36,
    0xcd, 0xc7, 0x57, 0xd3, 0x4c, 0x14, 0x94, 0x4a, 0xb6, 0x6c, 0x76, 0x32,
    0xd8, 0xa6, 0xb5, 0xce, 0x01, 0xb4, 0x2e, 0x7f, 0x16, 0x0a, 0x14, 0x16,
    0xcc, 0x1c, 0x07, 0x9c, 0x0d, 0x63, 0xe9, 0xb8, 0x48, 0x30, 0x56, 0xcf,
    0xc4, 0x79, 0x69, 0x31, 0x3f, 0x62, 0xbe, 0xc5, 0xac, 0xf9, 0xbd, 0x03,
    0x1a, 0x81, 0xf7, 0x0c, 0x70, 0x27, 0x07, 0x41, 0x7b, 0x68, 0x0e, 0xb1,
    0xf5, 0x29, 0x40, 0x97, 0xc7, 0xe1, 0xbb, 0xa0, 0x27, 0xa0, 0x35, 0x61,
    0x01, 0x6a, 0xa1, 0x50, 0x44, 0x86, 0x78, 0x5e, 0xa1, 0xb3, 0xcd, 0xbb,
    0x58, 0x0c, 0xd9, 0xaa, 0x4f, 0x48, 0x43, 0x7c, 0xc7, 0xbe, 0xac, 0xb0,
    0x9a, 0x77, 0x95, 0x45, 0x03, 0x81, 0x79, 0xe5, 0xed, 0xdd, 0xb0, 0xe9,
    0x17, 0x70, 0xcb, 0xca, 0x96, 0xc2, 0x86, 0x19, 0x8b, 0x81, 0xfe, 0x29,
    0xfc, 0xaf, 0x81, 0x12, 0xc0, 0x00, 0x78, 0x03, 0xb0, 0xa6, 0x3f, 0x69,
    0xf8, 0xba, 0x0d, 0x08, 0xac, 0xdc, 0xb0, 0xcf, 0x3c, 0x70, 0x05, 0x44,
    0x87, 0x1a, 0x96, 0xf0, 0x76, 0x6a, 0x73, 0xb2, 0x28, 0x60, 0xd6, 0x47,
    0x63, 0x01, 0xd4, 0xb9, 0x69, 0xbb, 0xb9, 0x9c, 0x68, 0x20, 0x0a, 0x5d,
    0x3b, 0xb5, 0xd5, 0x0e, 0x16, 0x32, 0x14, 0x10, 0x2d, 0x69, 0x7b, 0x1a,
    0x29, 0xc4, 0x26, 0x7e, 0x0f, 0x52, 0xfe, 0x99, 0x1e, 0xc0, 0xa9, 0x70,
    0x0c, 0xaf, 0xf0, 0x29, 0x40, 0x73, 0xa2, 0xf9, 0xc8, 0x90, 0xcf, 0x05,
    0xdf, 0x93, 0x50, 0x4c, 0x72, 0xa4, 0x8d, 0x56, 0xc7, 0x69, 0x29, 0x5b,
    0x40, 0xcd, 0xbb, 0x49, 0x33, 0xe4, 0x07, 0xf3, 0xdb, 0xc1, 0x9e, 0x50,
    0x11, 0x88, 0xaa, 0xc8, 0x01, 0xcf, 0x7b, 0x76, 0xe8, 0x16, 0x56, 0x9c,
    0x98, 0x9e, 0x04, 0x19, 0xd2, 0x96, 0x3b, 0x2f, 0x96, 0x8e, 0x95, 0x5f,
    0x65, 0xbf, 0x8e, 0x57, 0xc8, 0xff, 0xde, 0x99, 0x37, 0xdb, 0x02, 0x00,
    0x33, 0x5c, 0x02, 0x45, 0x56, 0x40, 0xda, 0xf8, 0x8e, 0xd7, 0xbd, 0xb5,
    0x65, 0xd7, 0xc2, 0xe1, 0xda, 0x78, 0xf2, 0xd8, 0xd0, 0x59, 0x21, 0x4b,
    0x77, 0x97, 0xe6, 0x7f, 0x69, 0xf7, 0x6b, 0xdf, 0xe3, 0xf8, 0x1f, 0x2e,
    0xd7, 0x7b, 0xa5, 0x0e, 0x1d, 0x1d, 0x94, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

static const char* const image7_data[] = { 
"20 14 26 1",
"i c #0000ff",
". c #09be1a",
"v c #1b1bff",
"# c #1cc119",
"h c #2726d8",
"n c #4b4bff",
"b c #50cb17",
"o c #5251e0",
"x c #53cc17",
"p c #56cc17",
"m c #6666ff",
"g c #757388",
"w c #918fa0",
"l c #9696ff",
"a c #9dd913",
"e c #a0d913",
"u c #a3da13",
"q c #a9a8ef",
"k c #aeaeff",
"r c #b7b7ff",
"s c #cfcfff",
"f c #d2cf28",
"j c #d4d4f7",
"c c #d7e411",
"d c #eae710",
"t c #ffffff",
"....................",
"........#aa#........",
".......bcddcb.......",
".....#adddddde#.....",
"....bcdfghhgfdcb....",
"..#adddgiiiigddde#..",
".bcddddjklmnoddddcb.",
".pcddddqrstttddddcp.",
"..#udddgiiivwdddu#..",
"....xcdfghhgfdcp....",
".....#uddddddu#.....",
".......xcddcp.......",
"........#uu#........",
"...................."};


/*
 *  Constructs a geodma_window as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
geodma_window::geodma_window( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl ),
      image2( (const char **) image2_data ),
      image3( (const char **) image3_data ),
      image4( (const char **) image4_data ),
      image5( (const char **) image5_data ),
      image7( (const char **) image7_data )
{
    (void)statusBar();
    QImage img;
    img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
    image0 = img;
    img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
    image1 = img;
    img.loadFromData( image6_data, sizeof( image6_data ), "PNG" );
    image6 = img;
    if ( !name )
	setName( "geodma_window" );
    setMinimumSize( QSize( 21, 21 ) );
    setMaximumSize( QSize( 750, 450 ) );
    setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    q_tab = new QTabWidget( centralWidget(), "q_tab" );
    q_tab->setGeometry( QRect( 10, 10, 700, 400 ) );
    q_tab->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, q_tab->sizePolicy().hasHeightForWidth() ) );
    q_tab->setMinimumSize( QSize( 700, 400 ) );
    q_tab->setMaximumSize( QSize( 700, 400 ) );
    q_tab->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont q_tab_font(  q_tab->font() );
    q_tab_font.setFamily( "Bitstream Vera Sans" );
    q_tab->setFont( q_tab_font ); 

    TabPage = new QWidget( q_tab, "TabPage" );

    textLabel1_11_2_2 = new QLabel( TabPage, "textLabel1_11_2_2" );
    textLabel1_11_2_2->setGeometry( QRect( 10, 60, 260, 18 ) );
    QFont textLabel1_11_2_2_font(  textLabel1_11_2_2->font() );
    textLabel1_11_2_2->setFont( textLabel1_11_2_2_font ); 

    q_pb_run_segmentation = new QPushButton( TabPage, "q_pb_run_segmentation" );
    q_pb_run_segmentation->setGeometry( QRect( 600, 330, 80, 24 ) );

    groupBox16 = new QGroupBox( TabPage, "groupBox16" );
    groupBox16->setGeometry( QRect( 10, 110, 510, 240 ) );

    q_fr_baatz = new QFrame( groupBox16, "q_fr_baatz" );
    q_fr_baatz->setGeometry( QRect( 180, 30, 150, 190 ) );
    q_fr_baatz->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    q_fr_baatz->setFrameShape( QFrame::StyledPanel );
    q_fr_baatz->setFrameShadow( QFrame::Raised );

    textLabel2_3_5 = new QLabel( q_fr_baatz, "textLabel2_3_5" );
    textLabel2_3_5->setGeometry( QRect( 10, 40, 89, 12 ) );
    QFont textLabel2_3_5_font(  textLabel2_3_5->font() );
    textLabel2_3_5_font.setPointSize( 7 );
    textLabel2_3_5->setFont( textLabel2_3_5_font ); 

    textLabel2_3_2_3 = new QLabel( q_fr_baatz, "textLabel2_3_2_3" );
    textLabel2_3_2_3->setGeometry( QRect( 10, 90, 59, 12 ) );
    QFont textLabel2_3_2_3_font(  textLabel2_3_2_3->font() );
    textLabel2_3_2_3_font.setPointSize( 7 );
    textLabel2_3_2_3->setFont( textLabel2_3_2_3_font ); 

    textLabel2_3_3_2 = new QLabel( q_fr_baatz, "textLabel2_3_3_2" );
    textLabel2_3_3_2->setGeometry( QRect( 80, 90, 41, 14 ) );
    QFont textLabel2_3_3_2_font(  textLabel2_3_3_2->font() );
    textLabel2_3_3_2_font.setPointSize( 7 );
    textLabel2_3_3_2->setFont( textLabel2_3_3_2_font ); 

    q_sb_baatz_scale = new QSpinBox( q_fr_baatz, "q_sb_baatz_scale" );
    q_sb_baatz_scale->setGeometry( QRect( 10, 110, 50, 24 ) );
    QFont q_sb_baatz_scale_font(  q_sb_baatz_scale->font() );
    q_sb_baatz_scale_font.setPointSize( 7 );
    q_sb_baatz_scale->setFont( q_sb_baatz_scale_font ); 
    q_sb_baatz_scale->setMaxValue( 10000 );
    q_sb_baatz_scale->setMinValue( 1 );
    q_sb_baatz_scale->setLineStep( 1 );
    q_sb_baatz_scale->setValue( 50 );

    q_sb_baatz_color = new QSpinBox( q_fr_baatz, "q_sb_baatz_color" );
    q_sb_baatz_color->setGeometry( QRect( 80, 110, 50, 24 ) );
    QFont q_sb_baatz_color_font(  q_sb_baatz_color->font() );
    q_sb_baatz_color_font.setPointSize( 7 );
    q_sb_baatz_color->setFont( q_sb_baatz_color_font ); 
    q_sb_baatz_color->setMaxValue( 100 );
    q_sb_baatz_color->setMinValue( 0 );
    q_sb_baatz_color->setLineStep( 1 );
    q_sb_baatz_color->setValue( 40 );

    q_sb_baatz_compactness = new QSpinBox( q_fr_baatz, "q_sb_baatz_compactness" );
    q_sb_baatz_compactness->setGeometry( QRect( 10, 60, 50, 24 ) );
    QFont q_sb_baatz_compactness_font(  q_sb_baatz_compactness->font() );
    q_sb_baatz_compactness_font.setPointSize( 7 );
    q_sb_baatz_compactness->setFont( q_sb_baatz_compactness_font ); 
    q_sb_baatz_compactness->setMaxValue( 100 );
    q_sb_baatz_compactness->setMinValue( 0 );
    q_sb_baatz_compactness->setLineStep( 1 );
    q_sb_baatz_compactness->setValue( 30 );

    textLabel2_3_4_2_2_2 = new QLabel( q_fr_baatz, "textLabel2_3_4_2_2_2" );
    textLabel2_3_4_2_2_2->setGeometry( QRect( 10, 10, 130, 17 ) );
    QFont textLabel2_3_4_2_2_2_font(  textLabel2_3_4_2_2_2->font() );
    textLabel2_3_4_2_2_2_font.setPointSize( 7 );
    textLabel2_3_4_2_2_2->setFont( textLabel2_3_4_2_2_2_font ); 

    q_fr_chessboard = new QFrame( groupBox16, "q_fr_chessboard" );
    q_fr_chessboard->setGeometry( QRect( 340, 30, 150, 190 ) );
    q_fr_chessboard->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    q_fr_chessboard->setFrameShape( QFrame::StyledPanel );
    q_fr_chessboard->setFrameShadow( QFrame::Raised );

    textLabel2_3_4_2_2_2_2 = new QLabel( q_fr_chessboard, "textLabel2_3_4_2_2_2_2" );
    textLabel2_3_4_2_2_2_2->setGeometry( QRect( 10, 10, 80, 16 ) );
    QFont textLabel2_3_4_2_2_2_2_font(  textLabel2_3_4_2_2_2_2->font() );
    textLabel2_3_4_2_2_2_2_font.setPointSize( 7 );
    textLabel2_3_4_2_2_2_2->setFont( textLabel2_3_4_2_2_2_2_font ); 

    textLabel2_3_5_2 = new QLabel( q_fr_chessboard, "textLabel2_3_5_2" );
    textLabel2_3_5_2->setGeometry( QRect( 10, 40, 89, 12 ) );
    QFont textLabel2_3_5_2_font(  textLabel2_3_5_2->font() );
    textLabel2_3_5_2_font.setPointSize( 7 );
    textLabel2_3_5_2->setFont( textLabel2_3_5_2_font ); 

    textLabel2_3_2_3_2 = new QLabel( q_fr_chessboard, "textLabel2_3_2_3_2" );
    textLabel2_3_2_3_2->setGeometry( QRect( 10, 90, 92, 14 ) );
    QFont textLabel2_3_2_3_2_font(  textLabel2_3_2_3_2->font() );
    textLabel2_3_2_3_2_font.setPointSize( 7 );
    textLabel2_3_2_3_2->setFont( textLabel2_3_2_3_2_font ); 

    q_sb_chessboard_lines = new QSpinBox( q_fr_chessboard, "q_sb_chessboard_lines" );
    q_sb_chessboard_lines->setGeometry( QRect( 10, 60, 50, 24 ) );
    QFont q_sb_chessboard_lines_font(  q_sb_chessboard_lines->font() );
    q_sb_chessboard_lines_font.setPointSize( 7 );
    q_sb_chessboard_lines->setFont( q_sb_chessboard_lines_font ); 
    q_sb_chessboard_lines->setMaxValue( 50000 );
    q_sb_chessboard_lines->setMinValue( 1 );
    q_sb_chessboard_lines->setLineStep( 1 );
    q_sb_chessboard_lines->setValue( 20 );

    q_sb_chessboard_columns = new QSpinBox( q_fr_chessboard, "q_sb_chessboard_columns" );
    q_sb_chessboard_columns->setGeometry( QRect( 10, 110, 50, 24 ) );
    QFont q_sb_chessboard_columns_font(  q_sb_chessboard_columns->font() );
    q_sb_chessboard_columns_font.setPointSize( 7 );
    q_sb_chessboard_columns->setFont( q_sb_chessboard_columns_font ); 
    q_sb_chessboard_columns->setMaxValue( 50000 );
    q_sb_chessboard_columns->setMinValue( 1 );
    q_sb_chessboard_columns->setLineStep( 1 );
    q_sb_chessboard_columns->setValue( 20 );

    q_fr_regiongrowing = new QFrame( groupBox16, "q_fr_regiongrowing" );
    q_fr_regiongrowing->setGeometry( QRect( 20, 30, 150, 190 ) );
    q_fr_regiongrowing->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    q_fr_regiongrowing->setFrameShape( QFrame::StyledPanel );
    q_fr_regiongrowing->setFrameShadow( QFrame::Raised );

    textLabel2_3_2_2_2 = new QLabel( q_fr_regiongrowing, "textLabel2_3_2_2_2" );
    textLabel2_3_2_2_2->setGeometry( QRect( 10, 90, 72, 12 ) );
    QFont textLabel2_3_2_2_2_font(  textLabel2_3_2_2_2->font() );
    textLabel2_3_2_2_2_font.setPointSize( 7 );
    textLabel2_3_2_2_2->setFont( textLabel2_3_2_2_2_font ); 

    q_le_regiongrowing_minimumarea = new QLineEdit( q_fr_regiongrowing, "q_le_regiongrowing_minimumarea" );
    q_le_regiongrowing_minimumarea->setGeometry( QRect( 10, 110, 51, 21 ) );
    QFont q_le_regiongrowing_minimumarea_font(  q_le_regiongrowing_minimumarea->font() );
    q_le_regiongrowing_minimumarea_font.setPointSize( 7 );
    q_le_regiongrowing_minimumarea->setFont( q_le_regiongrowing_minimumarea_font ); 

    q_le_regiongrowing_euclideandistance = new QLineEdit( q_fr_regiongrowing, "q_le_regiongrowing_euclideandistance" );
    q_le_regiongrowing_euclideandistance->setGeometry( QRect( 10, 60, 51, 21 ) );
    QFont q_le_regiongrowing_euclideandistance_font(  q_le_regiongrowing_euclideandistance->font() );
    q_le_regiongrowing_euclideandistance_font.setPointSize( 7 );
    q_le_regiongrowing_euclideandistance->setFont( q_le_regiongrowing_euclideandistance_font ); 

    textLabel2_3_4_3 = new QLabel( q_fr_regiongrowing, "textLabel2_3_4_3" );
    textLabel2_3_4_3->setGeometry( QRect( 10, 40, 97, 12 ) );
    QFont textLabel2_3_4_3_font(  textLabel2_3_4_3->font() );
    textLabel2_3_4_3_font.setPointSize( 7 );
    textLabel2_3_4_3->setFont( textLabel2_3_4_3_font ); 

    textLabel2_3_4_2_3 = new QLabel( q_fr_regiongrowing, "textLabel2_3_4_2_3" );
    textLabel2_3_4_2_3->setGeometry( QRect( 10, 10, 130, 17 ) );
    QFont textLabel2_3_4_2_3_font(  textLabel2_3_4_2_3->font() );
    textLabel2_3_4_2_3_font.setPointSize( 7 );
    textLabel2_3_4_2_3->setFont( textLabel2_3_4_2_3_font ); 

    q_cb_available_raster_layers_segmentation = new QComboBox( FALSE, TabPage, "q_cb_available_raster_layers_segmentation" );
    q_cb_available_raster_layers_segmentation->setGeometry( QRect( 10, 30, 250, 21 ) );
    QFont q_cb_available_raster_layers_segmentation_font(  q_cb_available_raster_layers_segmentation->font() );
    q_cb_available_raster_layers_segmentation_font.setPointSize( 7 );
    q_cb_available_raster_layers_segmentation->setFont( q_cb_available_raster_layers_segmentation_font ); 

    q_cb_segmentation_algorithm = new QComboBox( FALSE, TabPage, "q_cb_segmentation_algorithm" );
    q_cb_segmentation_algorithm->setGeometry( QRect( 10, 80, 250, 21 ) );
    QFont q_cb_segmentation_algorithm_font(  q_cb_segmentation_algorithm->font() );
    q_cb_segmentation_algorithm_font.setPointSize( 7 );
    q_cb_segmentation_algorithm->setFont( q_cb_segmentation_algorithm_font ); 

    textLabel1_11_2 = new QLabel( TabPage, "textLabel1_11_2" );
    textLabel1_11_2->setGeometry( QRect( 10, 10, 280, 18 ) );
    QFont textLabel1_11_2_font(  textLabel1_11_2->font() );
    textLabel1_11_2->setFont( textLabel1_11_2_font ); 
    q_tab->insertTab( TabPage, QString("") );

    TabPage_2 = new QWidget( q_tab, "TabPage_2" );

    textLabel1_6 = new QLabel( TabPage_2, "textLabel1_6" );
    textLabel1_6->setGeometry( QRect( 10, 230, 250, 18 ) );

    textLabel2_4 = new QLabel( TabPage_2, "textLabel2_4" );
    textLabel2_4->setGeometry( QRect( 10, 170, 250, 16 ) );

    textLabel1_5_2 = new QLabel( TabPage_2, "textLabel1_5_2" );
    textLabel1_5_2->setGeometry( QRect( 10, 120, 250, 17 ) );
    QFont textLabel1_5_2_font(  textLabel1_5_2->font() );
    textLabel1_5_2->setFont( textLabel1_5_2_font ); 

    textLabel2 = new QLabel( TabPage_2, "textLabel2" );
    textLabel2->setGeometry( QRect( 10, 60, 250, 16 ) );

    textLabel1_5 = new QLabel( TabPage_2, "textLabel1_5" );
    textLabel1_5->setGeometry( QRect( 10, 10, 250, 17 ) );
    QFont textLabel1_5_font(  textLabel1_5->font() );
    textLabel1_5->setFont( textLabel1_5_font ); 

    q_cb_polygon_layers = new QComboBox( FALSE, TabPage_2, "q_cb_polygon_layers" );
    q_cb_polygon_layers->setGeometry( QRect( 10, 30, 250, 24 ) );
    QFont q_cb_polygon_layers_font(  q_cb_polygon_layers->font() );
    q_cb_polygon_layers_font.setPointSize( 7 );
    q_cb_polygon_layers->setFont( q_cb_polygon_layers_font ); 

    q_cb_polygon_tables = new QComboBox( FALSE, TabPage_2, "q_cb_polygon_tables" );
    q_cb_polygon_tables->setGeometry( QRect( 10, 80, 250, 24 ) );
    QFont q_cb_polygon_tables_font(  q_cb_polygon_tables->font() );
    q_cb_polygon_tables_font.setPointSize( 7 );
    q_cb_polygon_tables->setFont( q_cb_polygon_tables_font ); 

    q_cb_cell_layers = new QComboBox( FALSE, TabPage_2, "q_cb_cell_layers" );
    q_cb_cell_layers->setGeometry( QRect( 10, 140, 250, 24 ) );
    QFont q_cb_cell_layers_font(  q_cb_cell_layers->font() );
    q_cb_cell_layers_font.setPointSize( 7 );
    q_cb_cell_layers->setFont( q_cb_cell_layers_font ); 

    q_cb_cell_tables = new QComboBox( FALSE, TabPage_2, "q_cb_cell_tables" );
    q_cb_cell_tables->setGeometry( QRect( 10, 190, 250, 24 ) );
    QFont q_cb_cell_tables_font(  q_cb_cell_tables->font() );
    q_cb_cell_tables_font.setPointSize( 7 );
    q_cb_cell_tables->setFont( q_cb_cell_tables_font ); 

    q_cb_raster_layers = new QComboBox( FALSE, TabPage_2, "q_cb_raster_layers" );
    q_cb_raster_layers->setGeometry( QRect( 10, 250, 250, 24 ) );
    QFont q_cb_raster_layers_font(  q_cb_raster_layers->font() );
    q_cb_raster_layers_font.setPointSize( 7 );
    q_cb_raster_layers->setFont( q_cb_raster_layers_font ); 

    groupBox10_2_2 = new QGroupBox( TabPage_2, "groupBox10_2_2" );
    groupBox10_2_2->setGeometry( QRect( 280, 20, 400, 254 ) );

    textEdit6_2_2 = new QTextEdit( groupBox10_2_2, "textEdit6_2_2" );
    textEdit6_2_2->setGeometry( QRect( 11, 19, 370, 180 ) );
    textEdit6_2_2->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont textEdit6_2_2_font(  textEdit6_2_2->font() );
    textEdit6_2_2->setFont( textEdit6_2_2_font ); 
    textEdit6_2_2->setLineWidth( 0 );
    textEdit6_2_2->setWordWrap( QTextEdit::WidgetWidth );
    textEdit6_2_2->setReadOnly( TRUE );

    q_pb_select_layers = new QPushButton( TabPage_2, "q_pb_select_layers" );
    q_pb_select_layers->setGeometry( QRect( 600, 330, 80, 26 ) );

    q_pb_reset_geodma = new QPushButton( TabPage_2, "q_pb_reset_geodma" );
    q_pb_reset_geodma->setEnabled( FALSE );
    q_pb_reset_geodma->setGeometry( QRect( 540, 330, 50, 26 ) );
    q_tab->insertTab( TabPage_2, QString("") );

    tab = new QWidget( q_tab, "tab" );

    q_tb_unselect_attribute_extract = new QToolButton( tab, "q_tb_unselect_attribute_extract" );
    q_tb_unselect_attribute_extract->setGeometry( QRect( 220, 190, 20, 22 ) );

    q_tb_select_attribute_extract = new QToolButton( tab, "q_tb_select_attribute_extract" );
    q_tb_select_attribute_extract->setGeometry( QRect( 220, 160, 20, 22 ) );

    q_tb_select_attribute_extract_all = new QToolButton( tab, "q_tb_select_attribute_extract_all" );
    q_tb_select_attribute_extract_all->setGeometry( QRect( 220, 120, 20, 22 ) );

    q_lb_attributes_extract = new QListBox( tab, "q_lb_attributes_extract" );
    q_lb_attributes_extract->setGeometry( QRect( 250, 40, 200, 310 ) );
    QFont q_lb_attributes_extract_font(  q_lb_attributes_extract->font() );
    q_lb_attributes_extract_font.setPointSize( 7 );
    q_lb_attributes_extract->setFont( q_lb_attributes_extract_font ); 
    q_lb_attributes_extract->setSelectionMode( QListBox::Multi );

    groupBox10_2 = new QGroupBox( tab, "groupBox10_2" );
    groupBox10_2->setGeometry( QRect( 470, 30, 210, 280 ) );

    textEdit6_2 = new QTextEdit( groupBox10_2, "textEdit6_2" );
    textEdit6_2->setGeometry( QRect( 11, 19, 180, 240 ) );
    textEdit6_2->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont textEdit6_2_font(  textEdit6_2->font() );
    textEdit6_2->setFont( textEdit6_2_font ); 
    textEdit6_2->setLineWidth( 0 );
    textEdit6_2->setWordWrap( QTextEdit::WidgetWidth );
    textEdit6_2->setReadOnly( TRUE );

    q_pb_extract_attributes = new QPushButton( tab, "q_pb_extract_attributes" );
    q_pb_extract_attributes->setGeometry( QRect( 600, 330, 80, 26 ) );

    q_lb_attributes_available = new QListBox( tab, "q_lb_attributes_available" );
    q_lb_attributes_available->setGeometry( QRect( 10, 40, 200, 310 ) );
    QFont q_lb_attributes_available_font(  q_lb_attributes_available->font() );
    q_lb_attributes_available_font.setPointSize( 7 );
    q_lb_attributes_available->setFont( q_lb_attributes_available_font ); 
    q_lb_attributes_available->setSelectionMode( QListBox::Multi );

    textLabel1_3_2 = new QLabel( tab, "textLabel1_3_2" );
    textLabel1_3_2->setGeometry( QRect( 10, 10, 260, 16 ) );
    q_tab->insertTab( tab, QString("") );

    tab_2 = new QWidget( q_tab, "tab_2" );

    q_tb_unselect_attribute_norm = new QToolButton( tab_2, "q_tb_unselect_attribute_norm" );
    q_tb_unselect_attribute_norm->setGeometry( QRect( 220, 190, 20, 22 ) );

    q_pb_normalize = new QPushButton( tab_2, "q_pb_normalize" );
    q_pb_normalize->setGeometry( QRect( 600, 330, 80, 26 ) );

    q_br_methods = new QButtonGroup( tab_2, "q_br_methods" );
    q_br_methods->setGeometry( QRect( 470, 30, 210, 280 ) );
    q_br_methods->setFrameShape( QButtonGroup::GroupBoxPanel );

    q_rb_mean = new QRadioButton( q_br_methods, "q_rb_mean" );
    q_rb_mean->setGeometry( QRect( 10, 50, 160, 20 ) );

    textEdit6_3 = new QTextEdit( q_br_methods, "textEdit6_3" );
    textEdit6_3->setGeometry( QRect( 11, 75, 180, 190 ) );
    textEdit6_3->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    textEdit6_3->setFrameShape( QTextEdit::NoFrame );
    textEdit6_3->setWordWrap( QTextEdit::WidgetWidth );

    q_rb_normal = new QRadioButton( q_br_methods, "q_rb_normal" );
    q_rb_normal->setGeometry( QRect( 10, 30, 150, 22 ) );
    q_rb_normal->setChecked( TRUE );

    q_tb_select_attribute_normalize_all = new QToolButton( tab_2, "q_tb_select_attribute_normalize_all" );
    q_tb_select_attribute_normalize_all->setGeometry( QRect( 220, 120, 20, 22 ) );

    q_tb_select_attribute_norm = new QToolButton( tab_2, "q_tb_select_attribute_norm" );
    q_tb_select_attribute_norm->setGeometry( QRect( 220, 160, 20, 22 ) );

    q_lb_attributes_norm = new QListBox( tab_2, "q_lb_attributes_norm" );
    q_lb_attributes_norm->setGeometry( QRect( 250, 40, 200, 310 ) );
    QFont q_lb_attributes_norm_font(  q_lb_attributes_norm->font() );
    q_lb_attributes_norm_font.setPointSize( 7 );
    q_lb_attributes_norm->setFont( q_lb_attributes_norm_font ); 
    q_lb_attributes_norm->setSelectionMode( QListBox::Multi );

    q_lb_attributes = new QListBox( tab_2, "q_lb_attributes" );
    q_lb_attributes->setGeometry( QRect( 10, 40, 200, 310 ) );
    QFont q_lb_attributes_font(  q_lb_attributes->font() );
    q_lb_attributes_font.setPointSize( 7 );
    q_lb_attributes->setFont( q_lb_attributes_font ); 
    q_lb_attributes->setSelectionMode( QListBox::Multi );

    textLabel1_3 = new QLabel( tab_2, "textLabel1_3" );
    textLabel1_3->setGeometry( QRect( 10, 10, 290, 16 ) );
    q_tab->insertTab( tab_2, QString("") );

    TabPage_3 = new QWidget( q_tab, "TabPage_3" );

    q_pb_associate_class = new QPushButton( TabPage_3, "q_pb_associate_class" );
    q_pb_associate_class->setGeometry( QRect( 600, 330, 80, 26 ) );

    textLabel1_9 = new QLabel( TabPage_3, "textLabel1_9" );
    textLabel1_9->setGeometry( QRect( 10, 6, 180, 21 ) );

    groupBox10 = new QGroupBox( TabPage_3, "groupBox10" );
    groupBox10->setGeometry( QRect( 470, 30, 210, 280 ) );

    textEdit6 = new QTextEdit( groupBox10, "textEdit6" );
    textEdit6->setGeometry( QRect( 11, 19, 180, 230 ) );
    textEdit6->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont textEdit6_font(  textEdit6->font() );
    textEdit6->setFont( textEdit6_font ); 
    textEdit6->setLineWidth( 0 );
    textEdit6->setWordWrap( QTextEdit::WidgetWidth );
    textEdit6->setReadOnly( TRUE );

    q_lb_training_classes = new QListBox( TabPage_3, "q_lb_training_classes" );
    q_lb_training_classes->setGeometry( QRect( 10, 110, 440, 240 ) );
    QFont q_lb_training_classes_font(  q_lb_training_classes->font() );
    q_lb_training_classes_font.setPointSize( 7 );
    q_lb_training_classes->setFont( q_lb_training_classes_font ); 
    q_lb_training_classes->setColumnMode( QListBox::Variable );
    q_lb_training_classes->setRowMode( QListBox::FitToWidth );

    textLabel1_9_2 = new QLabel( TabPage_3, "textLabel1_9_2" );
    textLabel1_9_2->setGeometry( QRect( 170, 10, 130, 16 ) );

    q_pb_create_class = new QPushButton( TabPage_3, "q_pb_create_class" );
    q_pb_create_class->setGeometry( QRect( 330, 40, 56, 26 ) );

    textLabel2_2 = new QLabel( TabPage_3, "textLabel2_2" );
    textLabel2_2->setGeometry( QRect( 10, 78, 430, 19 ) );

    q_pb_remove_class = new QPushButton( TabPage_3, "q_pb_remove_class" );
    q_pb_remove_class->setGeometry( QRect( 424, 40, 26, 26 ) );
    q_pb_remove_class->setPixmap( image0 );

    q_pb_edit_class = new QPushButton( TabPage_3, "q_pb_edit_class" );
    q_pb_edit_class->setGeometry( QRect( 393, 40, 26, 26 ) );

    q_cb_new_class_color = new QComboBox( FALSE, TabPage_3, "q_cb_new_class_color" );
    q_cb_new_class_color->setGeometry( QRect( 170, 40, 150, 24 ) );
    QFont q_cb_new_class_color_font(  q_cb_new_class_color->font() );
    q_cb_new_class_color_font.setPointSize( 7 );
    q_cb_new_class_color->setFont( q_cb_new_class_color_font ); 

    q_le_new_class_name = new QLineEdit( TabPage_3, "q_le_new_class_name" );
    q_le_new_class_name->setGeometry( QRect( 10, 40, 150, 26 ) );
    QFont q_le_new_class_name_font(  q_le_new_class_name->font() );
    q_le_new_class_name_font.setPointSize( 7 );
    q_le_new_class_name->setFont( q_le_new_class_name_font ); 
    q_tab->insertTab( TabPage_3, QString("") );

    TabPage_4 = new QWidget( q_tab, "TabPage_4" );

    textLabel1_4 = new QLabel( TabPage_4, "textLabel1_4" );
    textLabel1_4->setGeometry( QRect( 10, 7, 290, 21 ) );

    q_pb_plot = new QPushButton( TabPage_4, "q_pb_plot" );
    q_pb_plot->setGeometry( QRect( 600, 330, 80, 26 ) );

    q_frame = new QFrame( TabPage_4, "q_frame" );
    q_frame->setGeometry( QRect( 10, 40, 440, 310 ) );
    q_frame->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, q_frame->sizePolicy().hasHeightForWidth() ) );
    q_frame->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    q_frame->setFrameShape( QFrame::StyledPanel );
    q_frame->setFrameShadow( QFrame::Raised );

    groupBox1 = new QGroupBox( TabPage_4, "groupBox1" );
    groupBox1->setGeometry( QRect( 470, 30, 210, 280 ) );
    groupBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, groupBox1->sizePolicy().hasHeightForWidth() ) );

    textLabel1_2_2 = new QLabel( groupBox1, "textLabel1_2_2" );
    textLabel1_2_2->setGeometry( QRect( 10, 220, 94, 15 ) );

    textLabel1_2_3 = new QLabel( groupBox1, "textLabel1_2_3" );
    textLabel1_2_3->setGeometry( QRect( 8, 171, 180, 16 ) );

    textLabel1_2 = new QLabel( groupBox1, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 10, 70, 180, 17 ) );

    textLabel1 = new QLabel( groupBox1, "textLabel1" );
    textLabel1->setEnabled( TRUE );
    textLabel1->setGeometry( QRect( 10, 120, 180, 17 ) );

    textLabel1_12 = new QLabel( groupBox1, "textLabel1_12" );
    textLabel1_12->setEnabled( TRUE );
    textLabel1_12->setGeometry( QRect( 10, 20, 180, 17 ) );

    q_pb_spatialize_attribute_y = new QPushButton( groupBox1, "q_pb_spatialize_attribute_y" );
    q_pb_spatialize_attribute_y->setGeometry( QRect( 173, 89, 26, 24 ) );
    q_pb_spatialize_attribute_y->setPixmap( image1 );

    q_cb_visualizing_what = new QComboBox( FALSE, groupBox1, "q_cb_visualizing_what" );
    q_cb_visualizing_what->setEnabled( TRUE );
    q_cb_visualizing_what->setGeometry( QRect( 10, 40, 190, 24 ) );
    QFont q_cb_visualizing_what_font(  q_cb_visualizing_what->font() );
    q_cb_visualizing_what_font.setPointSize( 7 );
    q_cb_visualizing_what->setFont( q_cb_visualizing_what_font ); 

    q_sb_srate = new QSpinBox( groupBox1, "q_sb_srate" );
    q_sb_srate->setGeometry( QRect( 10, 240, 70, 24 ) );
    QFont q_sb_srate_font(  q_sb_srate->font() );
    q_sb_srate_font.setPointSize( 7 );
    q_sb_srate->setFont( q_sb_srate_font ); 
    q_sb_srate->setMaxValue( 100 );
    q_sb_srate->setMinValue( 1 );
    q_sb_srate->setLineStep( 1 );
    q_sb_srate->setValue( 5 );

    q_cb_xaxis = new QComboBox( FALSE, groupBox1, "q_cb_xaxis" );
    q_cb_xaxis->setGeometry( QRect( 10, 140, 190, 24 ) );
    QFont q_cb_xaxis_font(  q_cb_xaxis->font() );
    q_cb_xaxis_font.setPointSize( 7 );
    q_cb_xaxis->setFont( q_cb_xaxis_font ); 

    q_cb_labels = new QComboBox( FALSE, groupBox1, "q_cb_labels" );
    q_cb_labels->setGeometry( QRect( 8, 191, 190, 24 ) );
    QFont q_cb_labels_font(  q_cb_labels->font() );
    q_cb_labels_font.setPointSize( 7 );
    q_cb_labels->setFont( q_cb_labels_font ); 

    q_cb_yaxis = new QComboBox( FALSE, groupBox1, "q_cb_yaxis" );
    q_cb_yaxis->setGeometry( QRect( 10, 90, 160, 24 ) );
    QFont q_cb_yaxis_font(  q_cb_yaxis->font() );
    q_cb_yaxis_font.setPointSize( 7 );
    q_cb_yaxis->setFont( q_cb_yaxis_font ); 
    q_tab->insertTab( TabPage_4, QString("") );

    TabPage_5 = new QWidget( q_tab, "TabPage_5" );

    textLabel1_10 = new QLabel( TabPage_5, "textLabel1_10" );
    textLabel1_10->setGeometry( QRect( 470, 10, 180, 16 ) );

    q_tb_unselect_attribute_classify = new QToolButton( TabPage_5, "q_tb_unselect_attribute_classify" );
    q_tb_unselect_attribute_classify->setGeometry( QRect( 220, 120, 20, 22 ) );

    q_tb_select_attribute_classify_all = new QToolButton( TabPage_5, "q_tb_select_attribute_classify_all" );
    q_tb_select_attribute_classify_all->setGeometry( QRect( 220, 50, 20, 22 ) );
    QFont q_tb_select_attribute_classify_all_font(  q_tb_select_attribute_classify_all->font() );
    q_tb_select_attribute_classify_all->setFont( q_tb_select_attribute_classify_all_font ); 

    q_tb_select_attribute_classify = new QToolButton( TabPage_5, "q_tb_select_attribute_classify" );
    q_tb_select_attribute_classify->setGeometry( QRect( 220, 90, 20, 22 ) );

    q_lb_attributes_classify = new QListBox( TabPage_5, "q_lb_attributes_classify" );
    q_lb_attributes_classify->setGeometry( QRect( 250, 40, 200, 150 ) );
    QFont q_lb_attributes_classify_font(  q_lb_attributes_classify->font() );
    q_lb_attributes_classify_font.setPointSize( 7 );
    q_lb_attributes_classify->setFont( q_lb_attributes_classify_font ); 
    q_lb_attributes_classify->setSelectionMode( QListBox::Multi );

    q_pb_classify = new QPushButton( TabPage_5, "q_pb_classify" );
    q_pb_classify->setGeometry( QRect( 600, 330, 80, 26 ) );

    q_lb_attributes_candidate = new QListBox( TabPage_5, "q_lb_attributes_candidate" );
    q_lb_attributes_candidate->setGeometry( QRect( 10, 40, 200, 150 ) );
    QFont q_lb_attributes_candidate_font(  q_lb_attributes_candidate->font() );
    q_lb_attributes_candidate_font.setPointSize( 7 );
    q_lb_attributes_candidate->setFont( q_lb_attributes_candidate_font ); 
    q_lb_attributes_candidate->setSelectionMode( QListBox::Multi );

    textLabel1_3_3 = new QLabel( TabPage_5, "textLabel1_3_3" );
    textLabel1_3_3->setGeometry( QRect( 10, 10, 370, 16 ) );

    buttonGroup9 = new QButtonGroup( TabPage_5, "buttonGroup9" );
    buttonGroup9->setGeometry( QRect( 10, 200, 440, 150 ) );

    textLabel1_2_3_2_2 = new QLabel( buttonGroup9, "textLabel1_2_3_2_2" );
    textLabel1_2_3_2_2->setGeometry( QRect( 10, 20, 130, 16 ) );

    q_cb_classifying_what = new QComboBox( FALSE, buttonGroup9, "q_cb_classifying_what" );
    q_cb_classifying_what->setEnabled( TRUE );
    q_cb_classifying_what->setGeometry( QRect( 10, 40, 90, 24 ) );
    QFont q_cb_classifying_what_font(  q_cb_classifying_what->font() );
    q_cb_classifying_what_font.setPointSize( 7 );
    q_cb_classifying_what->setFont( q_cb_classifying_what_font ); 

    textLabel1_2_3_2 = new QLabel( buttonGroup9, "textLabel1_2_3_2" );
    textLabel1_2_3_2->setGeometry( QRect( 110, 20, 88, 16 ) );

    textLabel1_2_3_2_3 = new QLabel( buttonGroup9, "textLabel1_2_3_2_3" );
    textLabel1_2_3_2_3->setGeometry( QRect( 210, 20, 228, 16 ) );

    q_rb_create_legends = new QRadioButton( buttonGroup9, "q_rb_create_legends" );
    q_rb_create_legends->setGeometry( QRect( 10, 100, 70, 20 ) );
    q_rb_create_legends->setChecked( TRUE );
    buttonGroup9->insert( q_rb_create_legends, 2 );

    q_rb_create_themes = new QRadioButton( buttonGroup9, "q_rb_create_themes" );
    q_rb_create_themes->setGeometry( QRect( 10, 120, 70, 20 ) );
    q_rb_create_themes->setChecked( FALSE );
    buttonGroup9->insert( q_rb_create_themes, 2 );

    textLabel1_8 = new QLabel( buttonGroup9, "textLabel1_8" );
    textLabel1_8->setGeometry( QRect( 90, 80, 90, 16 ) );

    textLabel1_8_2 = new QLabel( buttonGroup9, "textLabel1_8_2" );
    textLabel1_8_2->setGeometry( QRect( 8, 80, 70, 16 ) );

    q_le_new_view = new QLineEdit( buttonGroup9, "q_le_new_view" );
    q_le_new_view->setGeometry( QRect( 90, 100, 110, 23 ) );
    QFont q_le_new_view_font(  q_le_new_view->font() );
    q_le_new_view_font.setPointSize( 7 );
    q_le_new_view->setFont( q_le_new_view_font ); 

    q_lb_ancillary_training_sets = new QListBox( buttonGroup9, "q_lb_ancillary_training_sets" );
    q_lb_ancillary_training_sets->setGeometry( QRect( 210, 40, 220, 100 ) );
    QFont q_lb_ancillary_training_sets_font(  q_lb_ancillary_training_sets->font() );
    q_lb_ancillary_training_sets_font.setPointSize( 7 );
    q_lb_ancillary_training_sets->setFont( q_lb_ancillary_training_sets_font ); 
    q_lb_ancillary_training_sets->setSelectionMode( QListBox::Multi );

    q_cb_training_labels = new QComboBox( FALSE, buttonGroup9, "q_cb_training_labels" );
    q_cb_training_labels->setGeometry( QRect( 110, 40, 90, 24 ) );
    QFont q_cb_training_labels_font(  q_cb_training_labels->font() );
    q_cb_training_labels_font.setPointSize( 7 );
    q_cb_training_labels->setFont( q_cb_training_labels_font ); 

    q_tb_classifiers = new QToolBox( TabPage_5, "q_tb_classifiers" );
    q_tb_classifiers->setGeometry( QRect( 470, 40, 210, 270 ) );
    q_tb_classifiers->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    q_tb_classifiers->setFrameShape( QToolBox::GroupBoxPanel );
    q_tb_classifiers->setCurrentIndex( 0 );

    page2 = new QWidget( q_tb_classifiers, "page2" );
    page2->setBackgroundMode( QWidget::PaletteBackground );

    textLabel1_7_3 = new QLabel( page2, "textLabel1_7_3" );
    textLabel1_7_3->setGeometry( QRect( 5, 42, 59, 20 ) );

    textLabel1_7_4 = new QLabel( page2, "textLabel1_7_4" );
    textLabel1_7_4->setGeometry( QRect( 5, 73, 59, 20 ) );

    textLabel1_7 = new QLabel( page2, "textLabel1_7" );
    textLabel1_7->setGeometry( QRect( 5, 166, 59, 20 ) );

    textLabel1_7_6 = new QLabel( page2, "textLabel1_7_6" );
    textLabel1_7_6->setGeometry( QRect( 5, 135, 59, 20 ) );

    textLabel1_7_5 = new QLabel( page2, "textLabel1_7_5" );
    textLabel1_7_5->setGeometry( QRect( 5, 104, 59, 20 ) );

    textLabel1_7_2 = new QLabel( page2, "textLabel1_7_2" );
    textLabel1_7_2->setGeometry( QRect( 5, 11, 59, 20 ) );

    q_sb_max_epochs = new QSpinBox( page2, "q_sb_max_epochs" );
    q_sb_max_epochs->setGeometry( QRect( 70, 11, 133, 20 ) );
    QFont q_sb_max_epochs_font(  q_sb_max_epochs->font() );
    q_sb_max_epochs_font.setPointSize( 7 );
    q_sb_max_epochs->setFont( q_sb_max_epochs_font ); 
    q_sb_max_epochs->setMaxValue( 1000 );
    q_sb_max_epochs->setMinValue( 1 );
    q_sb_max_epochs->setValue( 50 );

    q_le_radius = new QLineEdit( page2, "q_le_radius" );
    q_le_radius->setGeometry( QRect( 70, 42, 133, 20 ) );
    QFont q_le_radius_font(  q_le_radius->font() );
    q_le_radius_font.setPointSize( 7 );
    q_le_radius->setFont( q_le_radius_font ); 

    q_le_decreasing = new QLineEdit( page2, "q_le_decreasing" );
    q_le_decreasing->setGeometry( QRect( 70, 73, 133, 20 ) );
    QFont q_le_decreasing_font(  q_le_decreasing->font() );
    q_le_decreasing_font.setPointSize( 7 );
    q_le_decreasing->setFont( q_le_decreasing_font ); 

    q_le_alpha = new QLineEdit( page2, "q_le_alpha" );
    q_le_alpha->setGeometry( QRect( 70, 104, 133, 20 ) );
    QFont q_le_alpha_font(  q_le_alpha->font() );
    q_le_alpha_font.setPointSize( 7 );
    q_le_alpha->setFont( q_le_alpha_font ); 

    q_sb_map_height = new QSpinBox( page2, "q_sb_map_height" );
    q_sb_map_height->setGeometry( QRect( 70, 135, 133, 20 ) );
    QFont q_sb_map_height_font(  q_sb_map_height->font() );
    q_sb_map_height_font.setPointSize( 7 );
    q_sb_map_height->setFont( q_sb_map_height_font ); 
    q_sb_map_height->setMaxValue( 100 );
    q_sb_map_height->setMinValue( 1 );
    q_sb_map_height->setValue( 3 );

    q_sb_map_width = new QSpinBox( page2, "q_sb_map_width" );
    q_sb_map_width->setGeometry( QRect( 70, 166, 133, 20 ) );
    QFont q_sb_map_width_font(  q_sb_map_width->font() );
    q_sb_map_width_font.setPointSize( 7 );
    q_sb_map_width->setFont( q_sb_map_width_font ); 
    q_sb_map_width->setMaxValue( 100 );
    q_sb_map_width->setMinValue( 1 );
    q_sb_map_width->setValue( 3 );
    q_tb_classifiers->addItem( page2, "SOM" );

    page = new QWidget( q_tb_classifiers, "page" );
    page->setBackgroundMode( QWidget::PaletteBackground );

    textEdit5 = new QTextEdit( page, "textEdit5" );
    textEdit5->setGeometry( QRect( 5, 9, 198, 133 ) );
    textEdit5->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont textEdit5_font(  textEdit5->font() );
    textEdit5->setFont( textEdit5_font ); 
    textEdit5->setLineWidth( -1 );
    textEdit5->setWordWrap( QTextEdit::WidgetWidth );

    q_cb_save_c45 = new QCheckBox( page, "q_cb_save_c45" );
    q_cb_save_c45->setGeometry( QRect( 5, 148, 198, 19 ) );

    q_pb_load_c45 = new QPushButton( page, "q_pb_load_c45" );
    q_pb_load_c45->setGeometry( QRect( 5, 173, 198, 24 ) );
    q_tb_classifiers->addItem( page, "DECISION_TREE" );

    page_2 = new QWidget( q_tb_classifiers, "page_2" );
    page_2->setBackgroundMode( QWidget::PaletteBackground );

    textLabel1_7_6_2 = new QLabel( page_2, "textLabel1_7_6_2" );
    textLabel1_7_6_2->setGeometry( QRect( 5, 121, 93, 20 ) );

    textLabel1_7_3_2 = new QLabel( page_2, "textLabel1_7_3_2" );
    textLabel1_7_3_2->setGeometry( QRect( 5, 93, 93, 20 ) );

    textLabel1_7_2_2 = new QLabel( page_2, "textLabel1_7_2_2" );
    textLabel1_7_2_2->setGeometry( QRect( 5, 65, 93, 20 ) );

    q_sb_max_epochs_ann = new QSpinBox( page_2, "q_sb_max_epochs_ann" );
    q_sb_max_epochs_ann->setGeometry( QRect( 104, 65, 99, 20 ) );
    QFont q_sb_max_epochs_ann_font(  q_sb_max_epochs_ann->font() );
    q_sb_max_epochs_ann_font.setPointSize( 7 );
    q_sb_max_epochs_ann->setFont( q_sb_max_epochs_ann_font ); 
    q_sb_max_epochs_ann->setMaxValue( 10000 );
    q_sb_max_epochs_ann->setMinValue( 1 );
    q_sb_max_epochs_ann->setValue( 500 );

    q_le_learning_rate = new QLineEdit( page_2, "q_le_learning_rate" );
    q_le_learning_rate->setGeometry( QRect( 104, 93, 99, 20 ) );
    QFont q_le_learning_rate_font(  q_le_learning_rate->font() );
    q_le_learning_rate_font.setPointSize( 7 );
    q_le_learning_rate->setFont( q_le_learning_rate_font ); 

    q_sb_hidden_neurons = new QSpinBox( page_2, "q_sb_hidden_neurons" );
    q_sb_hidden_neurons->setGeometry( QRect( 104, 121, 99, 20 ) );
    QFont q_sb_hidden_neurons_font(  q_sb_hidden_neurons->font() );
    q_sb_hidden_neurons_font.setPointSize( 7 );
    q_sb_hidden_neurons->setFont( q_sb_hidden_neurons_font ); 
    q_sb_hidden_neurons->setMaxValue( 100 );
    q_sb_hidden_neurons->setMinValue( 1 );
    q_sb_hidden_neurons->setValue( 5 );

    q_cb_activation_function = new QComboBox( FALSE, page_2, "q_cb_activation_function" );
    q_cb_activation_function->setGeometry( QRect( 104, 149, 99, 20 ) );
    QFont q_cb_activation_function_font(  q_cb_activation_function->font() );
    q_cb_activation_function_font.setPointSize( 7 );
    q_cb_activation_function->setFont( q_cb_activation_function_font ); 

    textLabel1_7_7 = new QLabel( page_2, "textLabel1_7_7" );
    textLabel1_7_7->setGeometry( QRect( 5, 149, 93, 20 ) );

    textEdit5_2 = new QTextEdit( page_2, "textEdit5_2" );
    textEdit5_2->setGeometry( QRect( 2, 10, 200, 42 ) );
    textEdit5_2->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont textEdit5_2_font(  textEdit5_2->font() );
    textEdit5_2->setFont( textEdit5_2_font ); 
    textEdit5_2->setLineWidth( -1 );
    textEdit5_2->setWordWrap( QTextEdit::WidgetWidth );
    q_tb_classifiers->addItem( page_2, "NEURAL_NETWORKS" );
    q_tab->insertTab( TabPage_5, QString("") );

    TabPage_6 = new QWidget( q_tab, "TabPage_6" );

    buttonGroup8 = new QButtonGroup( TabPage_6, "buttonGroup8" );
    buttonGroup8->setGeometry( QRect( 10, 60, 240, 290 ) );

    textLabel1_13_5 = new QLabel( buttonGroup8, "textLabel1_13_5" );
    textLabel1_13_5->setGeometry( QRect( 10, 110, 60, 16 ) );

    textLabel1_13_2 = new QLabel( buttonGroup8, "textLabel1_13_2" );
    textLabel1_13_2->setGeometry( QRect( 10, 60, 80, 16 ) );

    q_te_confusion_matrix = new QTextEdit( buttonGroup8, "q_te_confusion_matrix" );
    q_te_confusion_matrix->setGeometry( QRect( 11, 130, 220, 150 ) );
    QFont q_te_confusion_matrix_font(  q_te_confusion_matrix->font() );
    q_te_confusion_matrix_font.setFamily( "Courier New" );
    q_te_confusion_matrix->setFont( q_te_confusion_matrix_font ); 

    q_pb_create_confusion_matrix = new QPushButton( buttonGroup8, "q_pb_create_confusion_matrix" );
    q_pb_create_confusion_matrix->setGeometry( QRect( 170, 40, 60, 60 ) );

    textLabel1_13 = new QLabel( buttonGroup8, "textLabel1_13" );
    textLabel1_13->setGeometry( QRect( 11, 20, 130, 16 ) );

    q_cb_classification_column = new QComboBox( FALSE, buttonGroup8, "q_cb_classification_column" );
    q_cb_classification_column->setGeometry( QRect( 10, 80, 150, 20 ) );
    QFont q_cb_classification_column_font(  q_cb_classification_column->font() );
    q_cb_classification_column_font.setPointSize( 7 );
    q_cb_classification_column->setFont( q_cb_classification_column_font ); 

    q_cb_reference_column = new QComboBox( FALSE, buttonGroup8, "q_cb_reference_column" );
    q_cb_reference_column->setGeometry( QRect( 11, 40, 150, 20 ) );
    QFont q_cb_reference_column_font(  q_cb_reference_column->font() );
    q_cb_reference_column_font.setPointSize( 7 );
    q_cb_reference_column->setFont( q_cb_reference_column_font ); 

    textLabel1_13_4 = new QLabel( TabPage_6, "textLabel1_13_4" );
    textLabel1_13_4->setGeometry( QRect( 10, 10, 240, 16 ) );

    buttonGroup8_2 = new QButtonGroup( TabPage_6, "buttonGroup8_2" );
    buttonGroup8_2->setGeometry( QRect( 260, 60, 420, 290 ) );

    q_pb_start_monte_carlo = new QPushButton( buttonGroup8_2, "q_pb_start_monte_carlo" );
    q_pb_start_monte_carlo->setGeometry( QRect( 330, 250, 80, 26 ) );

    q_lb_monte_carlo_attributes = new QListBox( buttonGroup8_2, "q_lb_monte_carlo_attributes" );
    q_lb_monte_carlo_attributes->setGeometry( QRect( 10, 90, 200, 150 ) );
    QFont q_lb_monte_carlo_attributes_font(  q_lb_monte_carlo_attributes->font() );
    q_lb_monte_carlo_attributes_font.setPointSize( 7 );
    q_lb_monte_carlo_attributes->setFont( q_lb_monte_carlo_attributes_font ); 
    q_lb_monte_carlo_attributes->setSelectionMode( QListBox::Multi );

    q_te_monte_carlo_output = new QTextEdit( buttonGroup8_2, "q_te_monte_carlo_output" );
    q_te_monte_carlo_output->setGeometry( QRect( 230, 90, 180, 150 ) );
    QFont q_te_monte_carlo_output_font(  q_te_monte_carlo_output->font() );
    q_te_monte_carlo_output_font.setFamily( "Courier New" );
    q_te_monte_carlo_output->setFont( q_te_monte_carlo_output_font ); 

    textLabel1_13_5_2 = new QLabel( buttonGroup8_2, "textLabel1_13_5_2" );
    textLabel1_13_5_2->setGeometry( QRect( 230, 70, 60, 16 ) );

    textLabel1_13_3_2 = new QLabel( buttonGroup8_2, "textLabel1_13_3_2" );
    textLabel1_13_3_2->setGeometry( QRect( 10, 70, 200, 16 ) );

    q_sb_monte_carlo_iterations = new QSpinBox( buttonGroup8_2, "q_sb_monte_carlo_iterations" );
    q_sb_monte_carlo_iterations->setGeometry( QRect( 160, 40, 50, 20 ) );
    QFont q_sb_monte_carlo_iterations_font(  q_sb_monte_carlo_iterations->font() );
    q_sb_monte_carlo_iterations_font.setPointSize( 7 );
    q_sb_monte_carlo_iterations->setFont( q_sb_monte_carlo_iterations_font ); 
    q_sb_monte_carlo_iterations->setMaxValue( 5000 );
    q_sb_monte_carlo_iterations->setMinValue( 100 );
    q_sb_monte_carlo_iterations->setValue( 300 );

    q_cb_monte_carlo_labels = new QComboBox( FALSE, buttonGroup8_2, "q_cb_monte_carlo_labels" );
    q_cb_monte_carlo_labels->setGeometry( QRect( 10, 40, 140, 20 ) );
    QFont q_cb_monte_carlo_labels_font(  q_cb_monte_carlo_labels->font() );
    q_cb_monte_carlo_labels_font.setPointSize( 7 );
    q_cb_monte_carlo_labels->setFont( q_cb_monte_carlo_labels_font ); 

    textLabel1_13_3 = new QLabel( buttonGroup8_2, "textLabel1_13_3" );
    textLabel1_13_3->setGeometry( QRect( 10, 20, 80, 16 ) );

    textLabel1_13_2_2 = new QLabel( buttonGroup8_2, "textLabel1_13_2_2" );
    textLabel1_13_2_2->setGeometry( QRect( 160, 20, 70, 20 ) );

    q_cb_show_only_kappas = new QCheckBox( TabPage_6, "q_cb_show_only_kappas" );
    q_cb_show_only_kappas->setGeometry( QRect( 490, 103, 180, 20 ) );
    q_cb_show_only_kappas->setChecked( TRUE );

    q_cb_validating_what = new QComboBox( FALSE, TabPage_6, "q_cb_validating_what" );
    q_cb_validating_what->setEnabled( TRUE );
    q_cb_validating_what->setGeometry( QRect( 10, 30, 130, 24 ) );
    QFont q_cb_validating_what_font(  q_cb_validating_what->font() );
    q_cb_validating_what_font.setPointSize( 7 );
    q_cb_validating_what->setFont( q_cb_validating_what_font ); 
    q_tab->insertTab( TabPage_6, QString("") );

    TabPage_7 = new QWidget( q_tab, "TabPage_7" );

    buttonGroup5 = new QButtonGroup( TabPage_7, "buttonGroup5" );
    buttonGroup5->setGeometry( QRect( 10, 10, 670, 350 ) );

    q_te_about = new QTextEdit( buttonGroup5, "q_te_about" );
    q_te_about->setGeometry( QRect( 10, 20, 500, 320 ) );
    q_te_about->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont q_te_about_font(  q_te_about->font() );
    q_te_about_font.setPointSize( 8 );
    q_te_about->setFont( q_te_about_font ); 
    q_te_about->setLineWidth( 0 );
    q_te_about->setWordWrap( QTextEdit::WidgetWidth );
    q_te_about->setReadOnly( TRUE );

    frame6 = new QFrame( buttonGroup5, "frame6" );
    frame6->setGeometry( QRect( 530, 210, 130, 130 ) );
    frame6->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    frame6->setFrameShape( QFrame::StyledPanel );
    frame6->setFrameShadow( QFrame::Raised );

    pixmapLabel4 = new QLabel( frame6, "pixmapLabel4" );
    pixmapLabel4->setGeometry( QRect( 50, 10, 35, 30 ) );
    pixmapLabel4->setPixmap( image2 );
    pixmapLabel4->setScaledContents( TRUE );

    pixmapLabel3 = new QLabel( frame6, "pixmapLabel3" );
    pixmapLabel3->setGeometry( QRect( 10, 10, 30, 30 ) );
    pixmapLabel3->setPixmap( image3 );
    pixmapLabel3->setScaledContents( TRUE );

    pixmapLabel5 = new QLabel( frame6, "pixmapLabel5" );
    pixmapLabel5->setGeometry( QRect( 10, 50, 90, 30 ) );
    pixmapLabel5->setPixmap( image4 );
    pixmapLabel5->setScaledContents( TRUE );

    pixmapLabel2 = new QLabel( frame6, "pixmapLabel2" );
    pixmapLabel2->setGeometry( QRect( 10, 90, 70, 30 ) );
    pixmapLabel2->setPixmap( image5 );
    pixmapLabel2->setScaledContents( TRUE );

    q_pm_xmas = new QLabel( frame6, "q_pm_xmas" );
    q_pm_xmas->setEnabled( TRUE );
    q_pm_xmas->setGeometry( QRect( 90, 90, 26, 30 ) );
    q_pm_xmas->setPixmap( image6 );
    q_pm_xmas->setScaledContents( TRUE );

    q_pm_flag = new QLabel( frame6, "q_pm_flag" );
    q_pm_flag->setEnabled( TRUE );
    q_pm_flag->setGeometry( QRect( 90, 100, 20, 14 ) );
    q_pm_flag->setPixmap( image7 );
    q_pm_flag->setScaledContents( TRUE );

    q_te_version = new QTextEdit( buttonGroup5, "q_te_version" );
    q_te_version->setGeometry( QRect( 530, 20, 130, 180 ) );
    q_te_version->setPaletteBackgroundColor( QColor( 239, 239, 239 ) );
    QFont q_te_version_font(  q_te_version->font() );
    q_te_version->setFont( q_te_version_font ); 
    q_te_version->setLineWidth( 0 );
    q_te_version->setWordWrap( QTextEdit::WidgetWidth );
    q_te_version->setReadOnly( TRUE );
    q_tab->insertTab( TabPage_7, QString("") );

    // toolbars

    languageChange();
    resize( QSize(723, 450).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( q_pb_normalize, SIGNAL( clicked() ), this, SLOT( q_pb_normalize_clicked() ) );
    connect( q_pb_extract_attributes, SIGNAL( clicked() ), this, SLOT( q_pb_extract_attributes_clicked() ) );
    connect( q_tb_select_attribute_norm, SIGNAL( clicked() ), this, SLOT( q_tb_select_attribute_norm_clicked() ) );
    connect( q_tb_unselect_attribute_norm, SIGNAL( clicked() ), this, SLOT( q_tb_unselect_attribute_norm_clicked() ) );
    connect( q_tb_select_attribute_extract, SIGNAL( clicked() ), this, SLOT( q_tb_select_attribute_extract_clicked() ) );
    connect( q_tb_unselect_attribute_extract, SIGNAL( clicked() ), this, SLOT( q_tb_unselect_attribute_extract_clicked() ) );
    connect( q_pb_plot, SIGNAL( clicked() ), this, SLOT( q_pb_plot_clicked() ) );
    connect( q_tb_select_attribute_classify, SIGNAL( clicked() ), this, SLOT( q_tb_select_attribute_classify_clicked() ) );
    connect( q_tb_unselect_attribute_classify, SIGNAL( clicked() ), this, SLOT( q_tb_unselect_attribute_classify_clicked() ) );
    connect( q_pb_classify, SIGNAL( clicked() ), this, SLOT( q_pb_classify_clicked() ) );
    connect( q_pb_select_layers, SIGNAL( clicked() ), this, SLOT( q_pb_select_layers_clicked() ) );
    connect( q_cb_polygon_layers, SIGNAL( highlighted(int) ), this, SLOT( q_cb_polygon_layers_highlighted(int) ) );
    connect( q_cb_cell_layers, SIGNAL( highlighted(int) ), this, SLOT( q_cb_cell_layers_highlighted(int) ) );
    connect( q_cb_raster_layers, SIGNAL( highlighted(int) ), this, SLOT( q_cb_raster_layers_highlighted(int) ) );
    connect( q_cb_visualizing_what, SIGNAL( highlighted(int) ), this, SLOT( q_cb_visualizing_what_highlighted(int) ) );
    connect( q_cb_classifying_what, SIGNAL( highlighted(int) ), this, SLOT( q_cb_classifying_what_highlighted(int) ) );
    connect( q_pb_associate_class, SIGNAL( clicked() ), this, SLOT( q_pb_associate_class_clicked() ) );
    connect( q_pb_create_class, SIGNAL( clicked() ), this, SLOT( q_pb_create_class_clicked() ) );
    connect( q_pb_remove_class, SIGNAL( clicked() ), this, SLOT( q_pb_remove_class_clicked() ) );
    connect( q_pb_load_c45, SIGNAL( clicked() ), this, SLOT( q_pb_load_c45_clicked() ) );
    connect( q_pb_spatialize_attribute_y, SIGNAL( clicked() ), this, SLOT( q_pb_spatialize_attribute_y_clicked() ) );
    connect( q_tb_select_attribute_extract_all, SIGNAL( clicked() ), this, SLOT( q_tb_select_attribute_extract_all_clicked() ) );
    connect( q_tb_select_attribute_normalize_all, SIGNAL( clicked() ), this, SLOT( q_tb_select_attribute_normalize_all_clicked() ) );
    connect( q_tb_select_attribute_classify_all, SIGNAL( released() ), this, SLOT( q_tb_select_attribute_classify_all_released() ) );
    connect( q_pb_run_segmentation, SIGNAL( clicked() ), this, SLOT( q_pb_run_segmentation_clicked() ) );
    connect( q_cb_segmentation_algorithm, SIGNAL( highlighted(int) ), this, SLOT( q_cb_segmentation_algorithm_highlighted(int) ) );
    connect( q_tab, SIGNAL( selected(const QString&) ), this, SLOT( q_tab_selected(const QString&) ) );
    connect( q_pb_edit_class, SIGNAL( clicked() ), this, SLOT( q_pb_edit_class_clicked() ) );
    connect( q_pb_create_confusion_matrix, SIGNAL( clicked() ), this, SLOT( q_pb_create_confusion_matrix_clicked() ) );
    connect( q_cb_validating_what, SIGNAL( highlighted(int) ), this, SLOT( q_cb_validating_what_highlighted(int) ) );
    connect( q_pb_start_monte_carlo, SIGNAL( clicked() ), this, SLOT( q_pb_start_monte_carlo_clicked() ) );
    connect( q_pb_reset_geodma, SIGNAL( clicked() ), this, SLOT( q_pb_reset_geodma_clicked() ) );

    // tab order
    setTabOrder( q_tab, q_cb_polygon_layers );
    setTabOrder( q_cb_polygon_layers, q_cb_polygon_tables );
    setTabOrder( q_cb_polygon_tables, q_cb_raster_layers );
    setTabOrder( q_cb_raster_layers, q_pb_select_layers );
    setTabOrder( q_pb_select_layers, q_lb_attributes_available );
    setTabOrder( q_lb_attributes_available, q_lb_attributes_extract );
    setTabOrder( q_lb_attributes_extract, textEdit6_2 );
    setTabOrder( textEdit6_2, q_pb_extract_attributes );
    setTabOrder( q_pb_extract_attributes, q_lb_attributes );
    setTabOrder( q_lb_attributes, q_lb_attributes_norm );
    setTabOrder( q_lb_attributes_norm, q_rb_normal );
    setTabOrder( q_rb_normal, q_pb_normalize );
    setTabOrder( q_pb_normalize, q_cb_xaxis );
    setTabOrder( q_cb_xaxis, q_cb_yaxis );
    setTabOrder( q_cb_yaxis, q_cb_labels );
    setTabOrder( q_cb_labels, q_sb_srate );
    setTabOrder( q_sb_srate, q_pb_plot );
    setTabOrder( q_pb_plot, q_le_new_class_name );
    setTabOrder( q_le_new_class_name, q_cb_new_class_color );
    setTabOrder( q_cb_new_class_color, q_pb_create_class );
    setTabOrder( q_pb_create_class, q_pb_remove_class );
    setTabOrder( q_pb_remove_class, q_lb_training_classes );
    setTabOrder( q_lb_training_classes, textEdit6 );
    setTabOrder( textEdit6, q_pb_associate_class );
    setTabOrder( q_pb_associate_class, q_lb_attributes_candidate );
    setTabOrder( q_lb_attributes_candidate, q_lb_attributes_classify );
    setTabOrder( q_lb_attributes_classify, q_le_new_view );
    setTabOrder( q_le_new_view, q_cb_training_labels );
    setTabOrder( q_cb_training_labels, q_pb_classify );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
geodma_window::~geodma_window()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void geodma_window::languageChange()
{
    setCaption( tr( "GeoDMA Plugin" ) );
    textLabel1_11_2_2->setText( tr( "Select the Segmentation Algorithm" ) );
    q_pb_run_segmentation->setText( tr( "Segment" ) );
    groupBox16->setTitle( tr( "Segmentation Algorithms and Parameters" ) );
    textLabel2_3_5->setText( tr( "Compactness (%)" ) );
    textLabel2_3_2_3->setText( tr( "Scale (> 0)" ) );
    textLabel2_3_3_2->setText( tr( "Color (%)" ) );
    textLabel2_3_4_2_2_2->setText( tr( "<b>Baatz & Shape</b> <i>(based)</i>" ) );
    textLabel2_3_4_2_2_2_2->setText( tr( "<b>Chessboard</b>" ) );
    textLabel2_3_5_2->setText( tr( "Number of Lines" ) );
    textLabel2_3_2_3_2->setText( tr( "Number of Columns" ) );
    textLabel2_3_2_2_2->setText( tr( "Minimum Area" ) );
    q_le_regiongrowing_minimumarea->setText( tr( "10" ) );
    QToolTip::add( q_le_regiongrowing_minimumarea, tr( "The minimum area expected for generated polygons." ) );
    q_le_regiongrowing_euclideandistance->setText( tr( "20" ) );
    QToolTip::add( q_le_regiongrowing_euclideandistance, tr( "The minimum Euclidean Distance between each segment feature." ) );
    textLabel2_3_4_3->setText( tr( "Euclidean Distance" ) );
    textLabel2_3_4_2_3->setText( tr( "<b>Bins et al. 1996</b><i> (based)</i>" ) );
    q_cb_segmentation_algorithm->clear();
    q_cb_segmentation_algorithm->insertItem( tr( "Region Growing" ) );
    q_cb_segmentation_algorithm->insertItem( tr( "Baatz & Shape" ) );
    q_cb_segmentation_algorithm->insertItem( tr( "Chessboard" ) );
    textLabel1_11_2->setText( tr( "Select the Layer with <i>Raster Data</i> to Segment" ) );
    q_tab->changeTab( TabPage, tr( "Segmentation" ) );
    textLabel1_6->setText( tr( "Select the Layer with <i>Raster Data</i>" ) );
    textLabel2_4->setText( tr( "Select the associated table to the <i>Cells</i>" ) );
    textLabel1_5_2->setText( tr( "Select the Layer with <i>Cells</i>" ) );
    textLabel2->setText( tr( "Select the associated table to the <i>Polygons</i>" ) );
    textLabel1_5->setText( tr( "Select the Layer with <i>Polygons</i>" ) );
    groupBox10_2_2->setTitle( tr( "This is GeoDMA - Geographic Data Mining Analyst" ) );
    textEdit6_2_2->setText( tr( "<p>To start using <b>GeoDMA</b>, select the input data.\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"<p>The Layer with <i>Polygons</i> contains objects from segmentation. You can import a shape file in TerraView menu <b>File -> Import Data</b>, or use <b>Segmentation</b>, in the left tab.\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"<p>The Layer with <i>Cells</i> contains a grid of cells. To create a grid, use TerraView menu <b>InfoLayer -> Create Cells</b>.\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"<p>The Layer with <i>Raster Data</i> contains images. To import images, use TerraView menu <b>File -> Raster Import</b>." ) );
    q_pb_select_layers->setText( tr( "OK" ) );
    q_pb_reset_geodma->setText( tr( "Reset" ) );
    q_tab->changeTab( TabPage_2, tr( "Input" ) );
    q_tb_unselect_attribute_extract->setText( tr( "<" ) );
    q_tb_select_attribute_extract->setText( tr( ">" ) );
    q_tb_select_attribute_extract_all->setText( tr( "All" ) );
    groupBox10_2->setTitle( tr( "Tips" ) );
    textEdit6_2->setText( tr( "<p align=\"left\">1. Features with prefix <span style=\"font-style:italic\">p_</span> are spatial (use Polygons), and with prefix <span style=\"font-style:italic\">c_</span> are cell-based (use Cells). </p>\n"
"<p align=\"left\">2. Features with prefix <span style=\"font-style:italic\">r</span> are spectral, combined with Polygons <span style=\"font-style:italic\">rp_</span> or with Cells <span style=\"font-style:italic\">rc_</span>.</p>\n"
"<p align=\"left\">3. If you perform Normalization in the next tab, normalized features will have the <i>_n</i> suffix.</p>\n"
"" ) );
    q_pb_extract_attributes->setText( tr( "Extract" ) );
    textLabel1_3_2->setText( tr( "Select features to extract" ) );
    q_tab->changeTab( tab, tr( "Features" ) );
    q_tb_unselect_attribute_norm->setText( tr( "<" ) );
    q_pb_normalize->setText( tr( "Normalize" ) );
    q_br_methods->setTitle( tr( "Method" ) );
    q_rb_mean->setText( tr( "z-Score (Mean 0, Std 1)" ) );
    textEdit6_3->setText( tr( "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:9pt;font-family:Bitstream Vera Sans\">\n"
"<p>Tip: Features from Polygons will be stored in the Polygons table, and from Cells in the Cells table, according the <span style=\"font-style:italic\">Inputs</span> tab.</p>\n"
"</body></html>\n"
"" ) );
    q_rb_normal->setText( tr( "Linear, interval [0, 1]" ) );
    q_tb_select_attribute_normalize_all->setText( tr( "All" ) );
    q_tb_select_attribute_norm->setText( tr( ">" ) );
    textLabel1_3->setText( tr( "Select features to normalize" ) );
    q_tab->changeTab( tab_2, tr( "Normalize" ) );
    q_pb_associate_class->setText( tr( "Associate" ) );
    textLabel1_9->setText( tr( "Type the <i>New Class</i> name" ) );
    groupBox10->setTitle( tr( "Tips" ) );
    textEdit6->setText( tr( "<p align=\"left\">To insert a new class, type the class name, choose one color and press <span style=\"font-style:italic\">Create</span>. After point some objects in TerraView window, choose one class on the left and click <span style=\"font-style:italic\">Associate</span> to set it to all pointed objects.</p>\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"<p align=\"left\">To remove one class from the list, select it and press the <span style=\"font-style:italic\">trash</span> button.</p>\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"" ) );
    textLabel1_9_2->setText( tr( "Set the color" ) );
    q_pb_create_class->setText( tr( "Create" ) );
    textLabel2_2->setText( tr( "Select one <i>Class</i>, associate it to pointed objects, or remove it" ) );
    q_pb_remove_class->setText( QString::null );
    q_pb_edit_class->setText( tr( "Edit" ) );
    q_le_new_class_name->setText( QString::null );
    q_tab->changeTab( TabPage_3, tr( "Train" ) );
    textLabel1_4->setText( tr( "ScatterPlot - Select <i>x</i> and <i>y</i> Axis" ) );
    q_pb_plot->setText( tr( "Plot" ) );
    groupBox1->setTitle( tr( "General Parameters" ) );
    textLabel1_2_2->setText( tr( "Sampling Rate" ) );
    textLabel1_2_3->setText( tr( "label" ) );
    textLabel1_2->setText( tr( "y" ) );
    textLabel1->setText( tr( "x" ) );
    textLabel1_12->setText( tr( "Visualizing" ) );
    q_pb_spatialize_attribute_y->setText( QString::null );
    QToolTip::add( q_pb_spatialize_attribute_y, tr( "Spatialize this attribute" ) );
    q_cb_visualizing_what->clear();
    q_cb_visualizing_what->insertItem( tr( "Polygons" ) );
    q_cb_visualizing_what->insertItem( tr( "Cells" ) );
    q_tab->changeTab( TabPage_4, tr( "Visualize" ) );
    textLabel1_10->setText( tr( "Choose algorithm" ) );
    q_tb_unselect_attribute_classify->setText( tr( "<" ) );
    q_tb_select_attribute_classify_all->setText( tr( "All" ) );
    q_tb_select_attribute_classify->setText( tr( ">" ) );
    q_pb_classify->setText( tr( "Classify" ) );
    textLabel1_3_3->setText( tr( "Select features to perform classification" ) );
    buttonGroup9->setTitle( tr( "General Parameters" ) );
    textLabel1_2_3_2_2->setText( tr( "Classifying" ) );
    q_cb_classifying_what->clear();
    q_cb_classifying_what->insertItem( tr( "Polygons" ) );
    q_cb_classifying_what->insertItem( tr( "Cells" ) );
    textLabel1_2_3_2->setText( tr( "Training Labels" ) );
    textLabel1_2_3_2_3->setText( tr( "Ancillary training sets <font size=\"-1\">(optional, layer->table)</font>" ) );
    q_rb_create_legends->setText( tr( "Legends" ) );
    q_rb_create_themes->setText( tr( "Themes" ) );
    textLabel1_8->setText( tr( "View name" ) );
    textLabel1_8_2->setText( tr( "Output as" ) );
    textLabel1_7_3->setText( tr( "Radius" ) );
    textLabel1_7_4->setText( tr( "Decreasing" ) );
    textLabel1_7->setText( tr( "Map Width" ) );
    textLabel1_7_6->setText( tr( "Map Height" ) );
    textLabel1_7_5->setText( tr( "Alpha" ) );
    textLabel1_7_2->setText( tr( "Max Epochs" ) );
    q_le_radius->setText( tr( "." ) );
    q_le_radius->setInputMask( tr( "#9.999; " ) );
    q_le_decreasing->setText( tr( "." ) );
    q_le_decreasing->setInputMask( tr( "#9.999; " ) );
    q_le_alpha->setText( tr( "." ) );
    q_le_alpha->setInputMask( tr( "#9.999; " ) );
    textEdit5->setText( tr( "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:9pt;font-family:Bitstream Vera Sans\">\n"
"<p>Select some the attributes, pick one 'Training Label' and press <span style=\"font-style:italic\">Classify</span>. To save the generated decision tree, check the line following <span style=\"font-style:italic\">Save Decision Tree</span>. To load a previously saved one, press <span style=\"font-style:italic\">Load Decision Tree</span>.</p>\n"
"</body></html>\n"
"" ) );
    q_cb_save_c45->setText( tr( "Save Decision Tree" ) );
    q_pb_load_c45->setText( tr( "Load Decision Tree" ) );
    textLabel1_7_6_2->setText( tr( "Hidden Neurons" ) );
    textLabel1_7_3_2->setText( tr( "Learning Rate" ) );
    textLabel1_7_2_2->setText( tr( "Max Epochs" ) );
    q_le_learning_rate->setText( tr( "." ) );
    q_le_learning_rate->setInputMask( tr( "#9.999; " ) );
    q_cb_activation_function->clear();
    q_cb_activation_function->insertItem( tr( "linear" ) );
    q_cb_activation_function->insertItem( tr( "logistic" ) );
    q_cb_activation_function->insertItem( tr( "quadratic" ) );
    q_cb_activation_function->setCurrentItem( 1 );
    textLabel1_7_7->setText( tr( "Act. Function" ) );
    textEdit5_2->setText( tr( "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:9pt;font-family:Bitstream Vera Sans\">\n"
"<p>Tip: You must use <span style=\"font-style:italic\">only</span> values normalized between [0,1].</p>\n"
"</body></html>\n"
"" ) );
    q_tab->changeTab( TabPage_5, tr( "Classify" ) );
    buttonGroup8->setTitle( tr( "Confusion Matrix" ) );
    textLabel1_13_5->setText( tr( "Output" ) );
    textLabel1_13_2->setText( tr( "Test Column" ) );
    q_pb_create_confusion_matrix->setText( tr( "Create" ) );
    textLabel1_13->setText( tr( "Reference Column" ) );
    textLabel1_13_4->setText( tr( "Select data type to validate" ) );
    buttonGroup8_2->setTitle( tr( "Monte Carlo Simulation (using Decision Trees)" ) );
    q_pb_start_monte_carlo->setText( tr( "Start" ) );
    textLabel1_13_5_2->setText( tr( "Output" ) );
    textLabel1_13_3_2->setText( tr( "Select features to simulate" ) );
    textLabel1_13_3->setText( tr( "Training Labels" ) );
    textLabel1_13_2_2->setText( tr( "Iterations" ) );
    q_cb_show_only_kappas->setText( tr( "Show only kappa values" ) );
    q_cb_validating_what->clear();
    q_cb_validating_what->insertItem( tr( "Polygons" ) );
    q_cb_validating_what->insertItem( tr( "Cells" ) );
    q_tab->changeTab( TabPage_6, tr( "Validate" ) );
    buttonGroup5->setTitle( tr( "GeoDMA is a plugin for TerraView, used for geographic data mining" ) );
    q_te_about->setText( trUtf8( "\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x6a\x75\x73\x74\x69\x66\x79\x22\x3e\x53\x6f\x6d\x65\x20\x61\x70\x70\x6c\x69\x63\x61\x74\x69\x6f\x6e\x20\x63\x68\x61\x72\x61\x63\x74\x65\x72\x69\x73\x74\x69\x63\x73\x20\x61\x72\x65\x20\x74\x68\x65\x20\x66\x6f\x6c\x6c\x6f\x77\x69\x6e\x67\x3a\x3c\x2f\x70\x3e\xa\x3c\x75\x6c\x3e\xa\x3c\x6c\x69\x3e\x54\x68\x65\x20\x75\x73\x65\x72\x20\x64\x65\x66\x69\x6e\x65\x73\x20\x74\x68\x65\x20\x63\x6c\x61\x73\x73\x65\x73\x20\x61\x6e\x64\x20\x64\x61\x74\x61\x20\x74\x68\x61\x74\x20\x77\x69\x6c\x6c\x20\x62\x65\x20\x75\x73\x65\x64\x20\x69\x6e\x20\x74\x68\x65\x20\x6d\x69\x6e\x69\x6e\x67\x20\x70\x72\x6f\x63\x65\x73\x73\xa\x3c\x6c\x69\x3e\x46\x72\x69\x65\x6e\x64\x6c\x79\x20\x61\x6e\x64\x20\x63\x6f\x6e\x74\x72\x6f\x6c\x6c\x65\x64\x20\x74\x72\x61\x69\x6e\x69\x6e\x67\x20\x70\x72\x6f\x63\x65\x73\x73\x20\x28\x3c\x69\x3e\x65\x2e\x67\x2e\x3c\x2f\x69\x3e\x20\x65\x6e\x61\x62\x6c\x69\x6e\x67\x20\x74\x68\x65\x20\x75\x73\x65\x72\x20\x74\x6f\x20\x6b\x6e\x6f\x77\x20\x77\x68\x61\x74\x20\x70\x61\x74\x74\x65\x72\x6e\x73\x20\x77\x65\x72\x65\x20\x73\x65\x6c\x65\x63\x74\x20\x74\x6f\x20\x74\x72\x61\x69\x6e\x69\x6e\x67\x29\xa\x3c\x6c\x69\x3e\x44\x65\x76\x65\x6c\x6f\x70\x65\x72\x73\x20\x61\x72\x65\x20\x61\x62\x6c\x65\x20\x74\x6f\x20\x69\x6e\x73\x65\x72\x74\x20\x6e\x65\x77\x20\x63\x6c\x61\x73\x73\x69\x66\x69\x65\x72\x73\x20\x61\x6e\x64\x20\x61\x74\x74\x72\x69\x62\x75\x74\x65\x73\xa\x3c\x2f\x75\x6c\x3e\xa\x3c\x70\x3e\x54\x68\x65\x20\x73\x65\x67\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x20\x61\x6c\x67\x6f\x72\x69\x74\x68\x6d\x73\x20\x61\x72\x65\x20\x62\x61\x73\x65\x64\x20\x6f\x6e\x3a\xa\x3c\x75\x6c\x3e\xa\x3c\x6c\x69\x3e\x42\x61\x61\x74\x7a\x2c\x20\x4d\x2e\x3b\x20\x53\x63\x68\xc3\xa4\x70\x65\x2c\x20\x41\x2e\x20\x4d\x75\x6c\x74\x69\x72\x65\x73\x6f\x6c\x75\x74\x69\x6f\x6e\x20\x73\x65\x67\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x3a\x20\x61\x6e\x20\x6f\x70\x74\x69\x6d\x69\x7a\x61\x74\x69\x6f\x6e\x20\x61\x70\x70\x72\x6f\x61\x63\x68\x20\x66\x6f\x72\x20\x68\x69\x67\x68\x20\x71\x75\x61\x6c\x69\x74\x79\x20\x6d\x75\x6c\x74\x69\x2d\x73\x63\x61\x6c\x65\x20\x69\x6d\x61\x67\x65\x20\x73\x65\x67\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x2e\x20\x3c\x62\x3e\x58\x49\x49\x20\x41\x6e\x67\x65\x77\x61\x6e\x64\x74\x65\x20\x47\x65\x6f\x67\x72\x61\x70\x68\x69\x73\x63\x68\x65\x20\x49\x6e\x66\x6f\x72\x6d\x61\x74\x69\x6f\x6e\x73\x76\x65\x72\x61\x72\x62\x65\x69\x74\x75\x6e\x67\x3c\x2f\x62\x3e\x2c\x20\x57\x69\x63\x68\x6d\x61\x6e\x6e\x20\x56\x65\x72\x6c\x61\x67\x2c\x20\x48\x65\x69\x64\x65\x6c\x62\x65\x72\x67\x2c\x20\x32\x30\x30\x30\x2e\xa\xa\x3c\x6c\x69\x3e\x42\x69\x6e\x73\x2c\x20\x4c\x2e\x53\x2e\x3b\x20\x46\x6f\x6e\x73\x65\x63\x61\x2c\x20\x4c\x2e\x4d\x2e\x47\x2e\x3b\x20\x45\x72\x74\x68\x61\x6c\x2c\x20\x47\x2e\x4a\x2e\x3b\x20\x49\x69\x2c\x20\x46\x2e\x4d\x2e\x20\x53\x61\x74\x65\x6c\x6c\x69\x74\x65\x20\x69\x6d\x61\x67\x65\x72\x79\x20\x73\x65\x67\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x3a\x20\x61\x20\x72\x65\x67\x69\x6f\x6e\x20\x67\x72\x6f\x77\x69\x6e\x67\x20\x61\x70\x70\x72\x6f\x61\x63\x68\x2e\x20\x3c\x62\x3e\x56\x49\x49\x49\x20\x42\x72\x61\x7a\x69\x6c\x69\x61\x6e\x20\x52\x65\x6d\x6f\x74\x65\x20\x53\x65\x6e\x73\x69\x6e\x67\x20\x53\x79\x6d\x70\x6f\x73\x69\x75\x6d\x3c\x2f\x62\x3e\x2c\x20\x53\x61\x6c\x76\x61\x64\x6f\x72\x2c\x20\x42\x72\x61\x7a\x69\x6c\x2e\x20\x70\x2e\x20\x36\x37\x37\x2d\x36\x38\x30\x2e\x20\x31\x39\x39\x36\x2e\xa\x3c\x2f\x75\x6c\x3e\xa\xa\x3c\x70\x3e\x4d\x6f\x72\x65\x20\x69\x6e\x66\x6f\x72\x6d\x61\x74\x69\x6f\x6e\x20\x61\x74\x20\x47\x65\x6f\x44\x4d\x41\x20\x6f\x66\x69\x63\x69\x61\x6c\x20\x68\x6f\x6d\x65\x20\x70\x61\x67\x65\x2e\x20\x3c\x61\x20\x68\x72\x65\x66\x3d\x22\x68\x74\x74\x70\x3a\x2f\x2f\x77\x77\x77\x2e\x64\x70\x69\x2e\x69\x6e\x70\x65\x2e\x62\x72\x2f\x67\x65\x6f\x64\x6d\x61\x2f\x22\x3e\x68\x74\x74\x70\x3a\x2f\x2f\x77\x77\x77\x2e\x64\x70\x69\x2e\x69\x6e\x70\x65\x2e\x62\x72\x2f\x67\x65\x6f\x64\x6d\x61\x2f\x3c\x2f\x61\x3e\x3c\x2f\x70\x3e\xa\x3c\x68\x72\x3e\xa\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x72\x69\x67\x68\x74\x22\x3e\x47\x65\x6f\x44\x4d\x41\x20\x74\x65\x61\x6d\x2e\x3c\x2f\x70\x3e\xa" ) );
    q_te_version->setText( QString::null );
    q_tab->changeTab( TabPage_7, tr( "About" ) );
}

