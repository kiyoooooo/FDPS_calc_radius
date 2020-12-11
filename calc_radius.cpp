#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <random>
#include <string.h>      //文字列の代入に使う
#include <bits/stdc++.h> //piの利用で必要(M_PI)
//complete
class ParticleInfo
{
public:
    uint32_t id;
    uint32_t type;
    /*position*/
    double posx;
    double posy;
    double posz;
    /*velocity*/
    double velx;
    double vely;
    double velz;
    /*結合*/
    uint32_t bond_pair[2];
    uint32_t bond_type[2];
    uint32_t nbond = 0;
    /*アングル*/
    uint32_t angle_pair[2][3];
    uint32_t angle_type[2];
    uint32_t nangle = 0;

    //sortを利用するために定義
    bool operator<(const ParticleInfo &another) const
    {
        //メンバ変数であるnum1で比較した結果を
        //この構造体の比較とする
        return id < another.id;
    }
};

int main(int argc, char *argv[])
{
    std::vector<ParticleInfo> pinfo;
    ParticleInfo temp_info;
    /*
    
    
    
    
    座標の読み込みを行う．*/
    std::ifstream ifs0(argv[1]);
    if (!ifs0)
    {
        std::cerr << "error0" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //いらないはじめの3行を捨てる．
    std::string delete_str[3];
    for (uint32_t i = 0; i < 3; i++)
    {
        std::getline(ifs0, delete_str[i]);
    }
    //ファイルの読み込み　粒子番号,粒子種は0から開始する．
    while (ifs0 >> temp_info.id >> temp_info.type >> temp_info.posx >> temp_info.posy >> temp_info.posz)
    {
        temp_info.id--;
        temp_info.type--;
        pinfo.push_back(temp_info);
    }
    ifs0.close();
    //はじめの文字列を読み込む
    double box_sx, box_sy, box_sz, box_ex, box_ey, box_ez, box_wt;
    sscanf(delete_str[0].c_str(), "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf",
           &box_sx, &box_sy, &box_sz, &box_ex, &box_ey, &box_ez, &box_wt);
    //    std::cout <<std::setprecision(10)<< box_sx << " " << box_sy << " " << box_sz << " " << box_ex << " " << box_ey << " " << box_ez << " " << box_wt << std::endl;
    std::sort(pinfo.begin(), pinfo.end());//classでオペレータを定義して利用している．

#if 0
    /*
    
    
    
    
    出力ファイルを生成する．*/
    //open file
    //pos_file

    FILE *fpo0;
    fpo0 = fopen(argv[5], "w");
    if (fpo0 == NULL)
    {
        printf("ERROR_initial_pos_lipid.cdv\n");
        return -1;
    }
    fprintf(fpo0, "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf\n", box_sx, box_sy, box_sz, box_ex, box_ey, box_ez, box_wt);
    for (int i = 1; i < 3; i++)
    {
        fprintf(fpo0, "%s \n", delete_str[i].c_str());
    }
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        fprintf(fpo0, "%d %d   %lf   %lf   %lf \n",
                double_vesicle_pinfo.at(i).id + 1,
                double_vesicle_pinfo.at(i).type + 1,
                double_vesicle_pinfo.at(i).posx,
                double_vesicle_pinfo.at(i).posy,
                double_vesicle_pinfo.at(i).posz);
    }
    fclose(fpo0);
    /*





    */
    //vel_file
    FILE *fpo1;
    fpo1 = fopen(argv[6], "w");
    if (fpo1 == NULL)
    {
        printf("ERROR_initial_vel_lipid.cdv\n");
        return -1;
    }
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        fprintf(fpo1, "%d   %lf   %lf   %lf \n",
                double_vesicle_pinfo.at(i).id + 1,
                double_vesicle_pinfo.at(i).velx,
                double_vesicle_pinfo.at(i).vely,
                double_vesicle_pinfo.at(i).velz);
    }
    fclose(fpo1);
    /*





    */
    //bond_fileここからはofstream記法で出力する．
    std::ofstream fpo2(argv[7], std::ios::out);
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        //"&&"条件で，double_vesicle_pinfo.at(i).nbond == 1のときに重複して出力してしまうのを防ぐ．
        if (double_vesicle_pinfo.at(i).nbond == 1 && double_vesicle_pinfo.at(i).id < double_vesicle_pinfo.at(i).bond_pair[0])
        {
            fpo2 << double_vesicle_pinfo.at(i).id + 1 << "   " << double_vesicle_pinfo.at(i).bond_pair[0] + 1 << "   " << double_vesicle_pinfo.at(i).bond_type[0] + 1 << std::endl;
        }
        else if (double_vesicle_pinfo.at(i).nbond == 2)
        {
            fpo2 << double_vesicle_pinfo.at(i).id + 1 << "   " << double_vesicle_pinfo.at(i).bond_pair[1] + 1 << "   " << double_vesicle_pinfo.at(i).bond_type[0] + 1 << std::endl;
        }
    }
    fpo2.close();
    /*





    */
    //bond_file ofstream記法で出力する．
    //angle_file
    std::ofstream fpo3(argv[8], std::ios::out);
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        if (double_vesicle_pinfo.at(i).nangle == 1)
        {
            fpo3 << double_vesicle_pinfo.at(i).angle_pair[0][0] + 1 << "   " << double_vesicle_pinfo.at(i).angle_pair[0][1] + 1 << "   " << double_vesicle_pinfo.at(i).angle_pair[0][2] + 1 << "   " << double_vesicle_pinfo.at(i).angle_type[0] + 1 << std::endl;
        }
    }
    fpo3.close();
#endif
    /*





    */

    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        if (double_vesicle_pinfo.at(i).type + 1 == 3)
            num_water++;
        else
            num_lipid++;
    }
    num = num_water + num_lipid;
    rho = num / (box_size_x * box_size_y * box_size_z);
    std::string filename0 = argv[9];
    std::ofstream writing_file0;
    writing_file0.open(filename0, std::ios::out);
    writing_file0 << "自分で決めるパラメータ" << std::endl;
    writing_file0 << "box_x_y = " << box_size_y / box_size_x << " (ボックスサイズのxが1としたときのyの比) " << std::endl;
    writing_file0 << "box_x_z = " << box_size_z / box_size_x << " (ボックスサイズのxが1としたときのzの比) " << std::endl;
    writing_file0 << std::endl;
    writing_file0 << "確定するパラメータ" << std::endl;
    writing_file0 << "num_water = " << num_water << " (水粒子の数) " << std::endl;
    writing_file0 << "num_lipid = " << num_lipid << " (脂質を構成する粒子の数) " << std::endl;
    writing_file0 << "num = " << num << " (すべての粒子数) " << std::endl;
    writing_file0 << "rho = " << rho << " (密度) " << std::endl;
    writing_file0 << "box_size_x = " << box_size_x << " (ボックスサイズxyz三辺が1：1：1の場合, box_size_x = box_size_y = box_size_z. ) " << std::endl;
    writing_file0 << "box_size_y = " << box_size_y << std::endl;
    writing_file0 << "box_size_z = " << box_size_z << std::endl;
    writing_file0 << std::endl;
    writing_file0 << "water : lipid = " << (double)num_water / (double)num * 100.0 << " : " << (double)num_lipid / (double)num * 100.0 << std::endl;
    writing_file0.close();

    return 0;
}