#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define INFINITY 10000
#define M 20                                                    /*最大顶点数*/
typedef struct {
	int num;                                                    /*景点代号*/
	char name[20];                                              /*景点名称*/
	char intro[200];                                            /*景点简介*/
}vertextype;

typedef int edgtype;                                            /*权值类型*/
typedef struct {                                                /*校园景点图结构体*/
	vertextype vexs[M];                                         /*顶点信息域*/
	edgtype edge[M][M];                                         /*邻接矩阵*/
	int vexNum, edgNum;                                         /*顶点数和边数*/
}mgraphtype;

int menu();                                                     /*主菜单*/
void Create_Map(mgraphtype *g);                                 /*从文件读取信息建立图*/
void Print_Map();                                               /*显示校园景点地图*/
int Judge_Input(int num);                                       /*判断输入的编号是否合理*/
void Search_Location(mgraphtype *g);                            /*景点信息查询*/
void ShortPath(mgraphtype *g);                                  /*求景点间最短路径*/
void Floyd_Print(mgraphtype *g, int sNum, int eNum);            /*递归打印两点间最短路径*/
void Shortpath_Print(mgraphtype *g);                            /*输出并打印两点间的最短路径*/
void Dfs_Print(mgraphtype *g, int sNum, int eNum);              /*深度优先遍历查询两景点间所有路径*/
void Allpath_Print(mgraphtype *g);                              /*查询两顶点间的所有路径并打印*/
void BestPath(mgraphtype *g);                                   /*多顶点间求最佳路径*/
void System_Exit(int *q);                                       /*退出系统*/

/*校园景点图的读取与创建*/
void Create_Map(mgraphtype *g)
{
	int i, j, k, e;
	FILE *rf;
	rf = fopen("map.txt", "r");
	if (rf)
	{
		fscanf(rf, "%d%d", &g->vexNum, &g->edgNum);
		for (i = 0; i < g->vexNum; i++)
			fscanf(rf, "%d%s%s", &g->vexs[i].num, g->vexs[i].name, g->vexs[i].intro);
		for (i = 0; i < g->vexNum; i++)
			for (j = 0; j < g->vexNum; j++)
			{
				if (i == j)
					g->edge[i][j] = 0;
				else
					g->edge[i][j] = INFINITY;
			}
		for (k = 0; k < g->edgNum; k++)
		{
			fscanf(rf, "%d%d%d", &i, &j, &e);
			g->edge[i][j] = g->edge[j][i] = e;
		}
		fclose(rf);
	}
	else
		g->edgNum = 0;
}

/*校园景点图的显示*/
void Dis_Map() {
	printf("\n                                『中北大学校园景点地图一览』\n\n");
	printf("                                                            ⑽瑾瑜国际会议中心         \n");
	printf("	                             ⑹校医院      ⑼游泳馆       ◎                       \n");
	printf("	                                ◎            ◎          ║                       \n");
	printf("	                                ╠══════╩═════╩══╦═══╗         \n");
	printf("	                                ║                              ║      ╚◎       \n");
	printf("	                    ⑸科艺苑    ║    ⑺图书馆                  ║      ⑿田园餐厅 \n");
	printf("	                      ◎        ║        ◎                    ║                 \n");
	printf("        ⑷二龙山◎════╩═════════╩══════════╣                 \n");
	printf("	            ║                            ║                    ◎                 \n");
	printf("	            ║                            ║                ⑾中北主楼             \n");
	printf("	            ║                            ◎                                       \n");
	printf("	            ◎⑵柏林园                ⑻主体育场                                   \n");
	printf("	            ╠════════◎                                                   \n");
	printf("	                ║        ⑶怡丁苑		                                            \n");
	printf("                    ║                                                                 \n");
	printf("                    ◎⑴中北大学一道门                                                 \n\n");
}

/*景点信息查询*/
void Search_Location(mgraphtype *g) {
	int s;
	do {
		printf("\n请输入你要查找的景点编号：");
		scanf("%d", &s);
	} while (Judge_Input(s));
	printf("\n景点名称：[%s]\n\n", g->vexs[s - 1].name);
	printf("景点简介： %s\n\n", g->vexs[s - 1].intro);
}

/*Floyd算法求两景点间的一条最短的路径*/
int dist[M][M];                                                    /*距离向量*/
int path[M][M];                                                    /*路径向量*/
void ShortPath(mgraphtype *g) {
	int i, j, k;
	for (i = 0; i < g->vexNum; i++)                                /*初始化距离向量矩阵与路径向量矩阵*/
		for (j = 0; j < g->vexNum; j++) {
			dist[i][j] = g->edge[i][j];
			if (i != j && dist[i][j] < INFINITY) path[i][j] = i;
			else path[i][j] = -1;                                  /*-1代表当前两点不可达*/
		}
	for (k = 0; k < g->vexNum; k++)                                /*递推求解每两景点的最短路径*/
		for (i = 0; i < g->vexNum; i++)
			for (j = 0; j < g->vexNum; j++)                        /*更新dist[i][j]的值*/
				if (dist[i][j] >(dist[i][k] + dist[k][j])) {
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = k;                                /*path用于记录最短路径上的经结点*/
				}
}

/*递归实现打印两点间的最短路径*/
void Floyd_Print(mgraphtype *g, int sNum, int eNum) {
	if (path[sNum][eNum] == -1 || path[sNum][eNum] == eNum || path[sNum][eNum] == sNum)
		return;
	else {
		Floyd_Print(g, sNum, path[sNum][eNum]);                 /*将中间点作为终点继续打印路径*/
		printf("%s->", g->vexs[path[sNum][eNum]].name);         /*打印中间景点名字*/
		Floyd_Print(g, path[sNum][eNum], eNum);                 /*将中间点作为起点继续打印路径*/
	}
}

/*输出并打印两点间的最短路径*/
void Shortpath_Print(mgraphtype *g) {
	int sNum, eNum;                                             /*起点编号，终点编号*/
	do {
		printf("\n请输入起点编号：");
		scanf("%d", &sNum);
	} while (Judge_Input(sNum));
	do {
		printf("\n请输入终点编号：");
		scanf("%d", &eNum);
	} while (Judge_Input(eNum));
	printf("\n%s到%s的最短距离是：%dm\n", g->vexs[--sNum].name, g->vexs[--eNum].name, dist[sNum][eNum]);
	printf("\n这是最佳游览路线：");
	printf("%s->", g->vexs[sNum].name);                                       /*输出路径上的起点*/
	Floyd_Print(g, sNum, eNum);                                               /*输出路径上的中间点*/
	printf("%s\n\n", g->vexs[eNum].name);                                     /*输出路径上的终点*/
}

/*深度优先遍历查询任意两个景点之间的所有路径*/
int pathStack[M];                                                             /*路径栈，存储路径信息*/
int top;                                                                      /*栈顶*/
int visited[M];                                                               /*入栈标记，防止形成回路*/
int count;                                                                    /*路径计数器*/
void Dfs_Print(mgraphtype *g, int sNum, int eNum) {
	int dis = 0;                                                              /*用于记录路径长度*/
	pathStack[top] = sNum;                                                    /*将本趟起点入栈*/
	top++;
	visited[sNum] = 1;                                                        /*将入栈点标记为已入栈*/
	for (int i = 0; i < g->vexNum; i++) {
		if (g->edge[sNum][i] > 0 && g->edge[sNum][i] != INFINITY && !visited[i]) {
			/*表明前一个入栈点与该点可达，且该点未入栈（未被访问）*/
			if (i == eNum) {                                                  /*如果深度遍历搜到了终点，就输出刚才的路径*/
				printf("第%d条路:", count++);
				for (int j = 0; j < top; j++) {
					printf("%s->", g->vexs[pathStack[j]].name);
					if (j < top - 1)
						dis = dis + g->edge[pathStack[j]][pathStack[j + 1]];        /*统计路径长度*/
				}
				dis = dis + g->edge[pathStack[top - 1]][eNum];                      /*最后一条路单独出来，因为enum不能入栈*/
				printf("%s\n", g->vexs[eNum].name);
				printf("总长度是：%dm\n\n", dis);
			}
			else {
				Dfs_Print(g, i, eNum);                                              /*如果该点不是终点,接着深度搜索*/
				top--;                                                              /*支路全被访问一遍后，顶点出栈*/
				visited[i] = 0;                                                     /*将出栈点标记为已出栈，允许下次访问*/
			}
		}
	}
}

/*查询任意两个景点之间的所有路径并打印*/
void Allpath_Print(mgraphtype *g) {
	int sNum, eNum;
	count = 1;                                                       /*路径计数器*/
	top = 0;                                                         /*栈顶*/
	memset(pathStack, 0, sizeof(pathStack));                         /*路径栈初始化*/
	memset(visited, 0, sizeof(visited));                             /*入栈标记初始化*/
	do {
		printf("\n请输入起点编号：");
		scanf("%d", &sNum);
	} while (Judge_Input(sNum));
	do {
		printf("\n请输入终点编号：");
		scanf("%d", &eNum);
	} while (Judge_Input(eNum));
	printf("\n");
	Dfs_Print(g, sNum - 1, eNum - 1);
}

/*多景点间求最佳路径*/
void BestPath(mgraphtype *g) {
	int vNum[M] = { 0 }, j = 1;                                       /*记录用户输入的编号信息*/
	int d = 0;                                                        /*统计全程总长*/
	printf("\n请输入你要游览的第%d个景点的编号（输入-1结束输入）：", j);
	scanf("%d", &vNum[j - 1]);
	while (vNum[j - 1] != -1 && j < 12) {
		while (Judge_Input(vNum[j - 1])) {
			printf("\n请输入你要游览的第%d个景点编号：", j);
			scanf("%d", &vNum[j - 1]);
		}
		if (vNum[j - 1] == -1) break;
		printf("\n请输入你要游览的第%d个景点编号：", ++j);
		scanf("%d", &vNum[j - 1]);
	}
	printf("\n这是最佳访问路径：");
	for (int i = 0; vNum[i] > 0 && vNum[i + 1] > 0; i++) {
		printf("%s->", g->vexs[vNum[i] - 1].name);                   /*输出路径上的起点*/
		Floyd_Print(g, vNum[i] - 1, vNum[i + 1] - 1);                /*利用Floyd算法*/
		d += dist[vNum[i] - 1][vNum[i + 1] - 1];
	}
	printf("%s\n\n", g->vexs[vNum[j - 2] - 1].name);                 /*输出路径上的终点*/
	printf("全程总长为：%dm\n\n", d);
}

/*主菜单*/
int menu() {
	int s;
	printf("\t\t                ┌───────────────────┐                \n");
	printf("\t\t┌──────├── 欢迎使用中北大学校园导游咨询系统 ──┤──────┐\n");
	printf("\t\t│              └───────────────────┘              │\n");
	printf("\t\t│                                                                      │\n");
	printf("\t\t│                                                                      │\n");
	printf("\t\t│   ┌────────────┐        ┌────────────┐   │\n");
	printf("\t\t│   │ 1.中北校园景点信息查询 │        │ 2.两景点间最短路径查询 │   │\n");
	printf("\t\t│   └────────────┘        └────────────┘   │\n");
	printf("\t\t│                                                                      │\n");
	printf("\t\t│   ┌────────────┐        ┌────────────┐   │\n");
	printf("\t\t│   │ 3.两景点间所有路径查询 │        │ 4.多景点间访问路线查询 │   │\n");
	printf("\t\t│   └────────────┘        └────────────┘   │\n");
	printf("\t\t│                                                                      │\n");
	printf("\t\t│   ┌────────────┐        ┌────────────┐   │\n");
	printf("\t\t│   │ 5.公告栏               │        │ 6.退出校园导游咨询系统 │   │\n");
	printf("\t\t│   └────────────┘        └────────────┘   │\n");
	printf("\t\t│                                                                      │\n");
	printf("\t\t└───────────────────────────────────┘\n\n");
	printf("\t\t请根据你的需求选择操作：");
	scanf_s("%d", &s);
	printf("\n\n");
	return s;
}

/*景点管理*/
struct 
{
	int close;                                               //景点关闭的标记
	char name[20];                                           //存放景点的名字
	char reason[100];                                        //存放景点关闭的原因
}Scenic_Manager[M];

void ScenicManager(mgraphtype *g)
{
	int n, i, no;
	system("cls"); Dis_Map();
	printf("请输入要关闭的景点编号: ");
	scanf("%d", &no);
	if (no < 1 || no > 12)
	{
		printf("输入的编号不正确，请重新输入\n");
	}
	else
	{
		Scenic_Manager[no].close = INFINITY;
		strcpy(Scenic_Manager[no].name, g->vexs[no].name);
		printf("\n");
		printf("请输入景点关闭的原因: \n");
		scanf("%s", Scenic_Manager[no].reason);
		printf("景点关闭成功！");
	}
}

/*判断输入的编号是否合理*/
int Judge_Input(int num)
{
	int i = 0;
	if (num == -1)
		return i;
	if (num < 1 || num>12)
	{
		printf("\n输入的编号有误，请输入1-12之间的数字！\n");
		i = 1;
	}
	else if(Scenic_Manager[num].close == INFINITY)
	{
		printf("\n该景点暂时关闭，无法游览。\n");
		printf("\n");
		printf("关闭原因: %s\n", Scenic_Manager[num].reason);
		i = 1;
	}

	return i;
}

/*管理员界面*/
void Manager()
{
	int n, num, no, i;
	mgraphtype *g;
	int quit = 0;
	FILE * rf;
	rf = fopen("Information.txt", "r");
	char info[200];
	printf("\t\t               ┌──────┐         ┌──────┐               \n");
	printf("\t\t               │ 1.发布公告 │         │ 2.景点管理 │               \n");
	printf("\t\t               └──────┘         └──────┘               \n");
	printf("请输入要选择的编号:\n");
	scanf("%d", &n);
		switch (n)
		{
		case 1:
			if (rf)
			{
				printf("输入要发布的公告信息数目: ");
				scanf("%d", &no);
				printf("\n");
				for (i = 1; i <= no; i++)
				{
					printf("请输入第%d条公告信息编号: ", i);
					scanf("%d", &num);
					printf("\n");
					printf("请输入第%d条公告信息内容: ", i);
					scanf("%s", info);
					printf("\n");
					fprintf(rf, "%d\n%s\n",num,info);
					printf("公告发布成功!\n\n");
					printf("\n");
				}
			}
			fclose(rf);
			Manager();
			break;
		case 2:
			ScenicManager(&g);
			break;
		case -1:
			break;
		default:
			printf("\t\t\t\t\t错误！没有该选项对应的操作。\n\n");
			break;
		}
}

/*用户登录*/
int Login()
{
	int n = 1, num;
	char count[20] = "NUC";
	char password[20] = "123";
	char count_input[20], password_input[20];
	printf("请输入账号:");
	scanf("%s", count_input);
	printf("\n");
	printf("请输入密码:");
	scanf("%s", password_input);
	printf("\n");
	if ((strcmp(count, count_input) == 0) && (strcmp(password, password_input) == 0))
	{
		printf("登录成功!\n");
		return n;
	}
	else printf("输入的账号或密码有误,请重新输入!\n\n");
	Login();
}

/*中北大学校园导游咨询系统登录系统*/
int Login_System()
{
	int num = 0;
	printf("\t\t                      ┌───────────────────┐                \n");
	printf("\t\t                    ├── 欢迎使用中北大学校园导游咨询系统 ──┤              \n");
	printf("\t\t                      └───────────────────┘                \n");
	printf("\t\t                                       请登录                                   \n");
	printf("\t\t             ┌───────────┐       ┌───────────┐         \n");
	printf("\t\t             │     1.管理员登录     │       │     2.游客登录       │         \n");
	printf("\t\t             └───────────┘       └───────────┘         \n");
	printf("请输入要选择的编号:\n");
	scanf("%d", &num);
	switch (num)
	{
	case 1:
		system("cls"); Login(); Manager(); break;
	case 2:
		system("cls"); Login(); break;
	default:
		printf("\t\t\t\t\t错误！没有该选项对应的操作。\n\n");
		break;
	}
	return 1;
}

/*公告栏*/
void Information_print()
{
	FILE *rf;
	char a[100];
	int n,i;
	rf = fopen("Information.txt", "r");
	if (rf)
	{
		printf("公告内容如下:\n\n");
		for (i = 0;i < 2; i++)
		{
			fscanf(rf, "%d%s", &n, a);
			printf("%d:%s\n\n", n, a);
		}
	}
	fclose(rf);
}

/*退出菜单栏*/
void System_Exit(int *quit) {
	*quit = 1;
	printf("\t\t                ┌───────────────────┐                \n");
	printf("\t\t              ├──        欢迎下次使用，谢谢！      ──┤              \n");
	printf("\t\t                └───────────────────┘                \n");
	printf("\t\t                  *     *        *       *         * * *                  \n");
	printf("\t\t                  * *   *        *       *       *       *                \n");
	printf("\t\t                  *  *  *        *       *       *                        \n");
	printf("\t\t                  *   * *        *       *       *       *                \n");
	printf("\t\t                  *     *          * * *           * * *                  \n");
}

/*主函数*/
void main() {
	int quit = 0;
	mgraphtype g;
	Create_Map(&g);                                                              /*从文件读取信息建立图*/
	ShortPath(&g);                                                               /*Floyd求出dist与path*/
	if (Login_System()) {
		while (!quit) {                                                          /*系统退出条件满足判定*/
			switch (menu()) {                                                    /*打印主菜单*/
			case 1:system("cls"); Dis_Map(); Search_Location(&g); break;         /*中北校园景点信息查询*/
			case 2:system("cls"); Dis_Map(); Shortpath_Print(&g); break;         /*两景点间最短路径查询*/
			case 3:system("cls"); Dis_Map(); Allpath_Print(&g); break;           /*两景点间所有路径查询*/
			case 4:system("cls"); Dis_Map(); BestPath(&g); break;                /*多景点间访问路线查询*/
			case 5:system("cls"); Dis_Map(); Information_print(); break;         /*公告栏查看*/
			case 6:system("cls"); System_Exit(&quit); break;                     /*退出校园导游咨询系统*/
			default:printf("\t\t\t\t\t错误！没有该选项对应的操作。\n\n");
			}
			system("pause");
			system("cls");                                                           /*清屏*/
		}
	}
}