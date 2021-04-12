#include <bits/stdc++.h>
using namespace std;


double mult[3][5] = {
    0, 1, 1.1, 1.2, 1.2,
    1, 1, 1.2, 1.2, 1.1,
    1, 1, 1, 1, 1
};

/*
double mult[3][5] = {
    1, 1, 1.3, 1.3, 1.5,
    1.4, 1.4, 1.8, 1.8, 2.0,
    1, 1, 1, 1, 1
};*/

/*
double mult[3][5] = {
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
};*/

struct Team{
    string id;
    int dmg;
    string use[5];
    Team(string id_, int dmg_, string* use_)
    {
        id = id_;
        dmg = dmg_;
        for(int i = 0; i < 5; i++)
        {
            use[i] = use_[i];
        }
    }
    int getphase()
    {
        map<char, int> mp = {
            {'a',0},
            {'b',1},
            {'c',2},
            {'A',0},
            {'B',1},
            {'C',2},
        };
        return mp[id[0]];
    }
    int getbossid()
    {
        for(int i = 0; i < id.size(); i++)
        {
            if(isdigit(id[i]))
            {
                return id[i]-'1';
            }
        }
        return 0;
    }
    double getmult()
    {
        return mult[getphase()][getbossid()];
    }
};

vector<string> donthave = {"´ºÃ¨","Çé²¡","Ê¥Ç§"};//"Ê¥´¸"

vector<string> iused = {};//"b201"

int bcnt(int a)
{
    return __builtin_popcount(a);
}

bool checkteams(Team& a, Team& b, Team& c)
{
    vector<string> all;
    for(int i = 0; i < 5; i++)
    {
        all.push_back(a.use[i]);
        all.push_back(b.use[i]);
        all.push_back(c.use[i]);
    }
    sort(all.begin(), all.end());
    all.erase(unique(all.begin(), all.end()), all.end());
    int bit[3] = {};
    int ld[3] = {};
    int lk[3] = {};
    int dbit = 0;
    for(int i = 0; i < 5; i++)
    {
        bit[0] |= 1 << (find(all.begin(), all.end(), a.use[i]) - all.begin());
        bit[1] |= 1 << (find(all.begin(), all.end(), b.use[i]) - all.begin());
        bit[2] |= 1 << (find(all.begin(), all.end(), c.use[i]) - all.begin());
    }
    for(auto f: donthave)
    {
        dbit |= 1 << (find_if(all.begin(), all.end(), [&](const string& s){return f == s;}) - all.begin());
    }

    for(int i = 0; i < 3; i++)
    {
        if(bcnt(bit[i] & dbit) > 1) return false;
        if(bcnt(bit[i] & dbit) == 1)
        {
            //cout<<all[__builtin_ctz(bit[i] & dbit)]<<endl;
            ld[i] = 1;
        }
    }
    for(int i = 0; i < 9; i++)
    {
        if(lk[i%3]) continue;
        int xc = bcnt(bit[i%3] & bit[(i+1)%3]);
        if(xc + ld[i%3] + ld[(i+1)%3] > 2) return false;
        if(xc + ld[i%3] + ld[(i+1)%3] == 2) {
            ld[i%3] = ld[(i+1)%3] = 1;
            lk[i%3] = 1;
        }
    }
    return true;
}

int main()
{
    for(auto sx: donthave)
    {
        cout<<sx<<' ';
    }
    cout<<endl;
    freopen("pcr_in.txt", "r", stdin);
    vector<Team> teams;
    string line,s;
    while(getline(cin,line,'\n'))
    {
        string bossid;
        string use[5];
        int dmg = 0;
        stringstream ss(line);
        if(!(ss>>s>>bossid>>use[0]>>use[1]>>use[2]>>use[3]>>use[4]>>dmg))
            continue;
        if(!(s[0]>='A'&& s[0]<='E')
           || !(s[1]>='1' &&s[1] <= '5'))
        {
            continue;
        }

        teams.push_back(Team(bossid, dmg, use));
    }
    cout<<teams.size()<<endl;
    vector<tuple<double, int, int, int> > res;
    for(int i = 0; i < teams.size(); i++)
    {
        if(iused.size() >= 1 && iused[0] != teams[i].id) continue;

        for(int j = 0; j < (iused.size() ? teams.size() : i); j++)
        {
            for(int k = 0; k < j; k++)
            {
                double dmg = teams[i].dmg * teams[i].getmult()
                            + teams[j].dmg * teams[j].getmult()
                            + teams[k].dmg * teams[k].getmult();
                if(checkteams(teams[i], teams[j], teams[k]))
                {
                    res.push_back(make_tuple(dmg, i, j, k));
                }
            }
        }
    }
    sort(res.begin(), res.end());
    reverse(res.begin(), res.end());
    for(int i = 0; i < min(50, (int)res.size()); i++)
    {
        int dmg = get<0>(res[i]);
        int x = get<1>(res[i]);
        int y = get<2>(res[i]);
        int z = get<3>(res[i]);
        cout<<dmg<<' ';
        cout<<teams[x].id<<' ';
        cout<<teams[y].id<<' ';
        cout<<teams[z].id<<' ';
        cout<<"[ ";
        for(int j = 0; j < 5; j++)
            cout<<teams[x].use[j]<<' ';
        cout<<"] ";
        cout<<"[ ";
        for(int j = 0; j < 5; j++)
            cout<<teams[y].use[j]<<' ';
        cout<<"] ";
        cout<<"[ ";
        for(int j = 0; j < 5; j++)
            cout<<teams[z].use[j]<<' ';
        cout<<"] ";
        cout<<endl;
    }
    return 0;
}
