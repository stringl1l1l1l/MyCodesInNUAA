#include <bits/stdc++.h>

using namespace std;
const int MAXN = 500;
const int MAXM = 500;
const int MAXQ = 5000;

struct Authority
{
    unordered_set<string> opNames;
    unordered_set<string> resTypes;
    unordered_set<string> resNames;
    Authority() {}
    Authority(unordered_set<string> &opNames, unordered_set<string> &resTypes, unordered_set<string> &resNames)
        : opNames(opNames), resTypes(resTypes), resNames(resNames) {}
};

unordered_map<string, unordered_set<string>> user_role;
unordered_map<string, unordered_set<string>> group_role;
unordered_map<string, Authority> role_auth;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, q;
    int size = 0;
    int ans[MAXQ] = {0};
    cin >> n >> m >> q;
    for (int i = 0; i < n; i++)
    {
        string roleName;
        unordered_set<string> opNames, resTypes, resNames;
        int opCount = 0, resTypeCount = 0, resNameCount = 0;
        cin >> roleName >> opCount;
        for (int j = 0; j < opCount; j++)
        {
            string opName;
            cin >> opName;
            opNames.insert(opName);
        }
        cin >> resTypeCount;
        for (int j = 0; j < resTypeCount; j++)
        {
            string resType;
            cin >> resType;
            resTypes.insert(resType);
        }
        cin >> resNameCount;
        for (int j = 0; j < resNameCount; j++)
        {
            string resName;
            cin >> resName;
            resNames.insert(resName);
        }
        Authority auth(opNames, resTypes, resNames);
        role_auth.insert(make_pair(roleName, auth));
    }

    for (int i = 0; i < m; i++)
    {
        string roleName;

        int objCount = 0;
        cin >> roleName >> objCount;
        for (int j = 0; j < objCount; j++)
        {
            char objType;
            string objName;
            cin >> objType >> objName;
            if (objType == 'u')
                user_role[objName].insert(roleName);
            else
                group_role[objName].insert(roleName);
        }
    }

    for (int i = 0; i < q; i++)
    {
        string userName, opName, resType, resName;
        unordered_set<string> roleNames;
        int groupCount = 0;
        cin >> userName >> groupCount;

        //根据用户名添加角色
        if (user_role.find(userName) != user_role.end())
        {
            unordered_set<string> roles = user_role[userName];
            for (auto role : roles)
                roleNames.insert(role);
        }
        //根据用户组添加角色
        for (int j = 0; j < groupCount; j++)
        {
            string groupName;
            cin >> groupName;
            if (group_role.find(groupName) != group_role.end())
            {
                unordered_set<string> roles = group_role[groupName];
                for (auto role : roles)
                    roleNames.insert(role);
            }
        }

        int flag = 0;
        cin >> opName >> resType >> resName;

        for (auto roleName : roleNames)
        {
            Authority auth;
            if (role_auth.find(roleName) != role_auth.end())
                auth = role_auth[roleName];
            else
                continue;
            if (auth.opNames.find(opName) == auth.opNames.end() && auth.opNames.find("*") == auth.opNames.end())
                continue;
            else if (auth.resTypes.find(resType) == auth.resTypes.end() && auth.resTypes.find("*") == auth.resTypes.end())
                continue;
            else if (!auth.resNames.empty() && auth.resNames.find(resName) == auth.resNames.end())
                continue;
            else
                flag = 1;
        }
        ans[size++] = flag;
    }
    for (int i = 0; i < size; i++)
    {
        cout << ans[i];
        if (i < size - 1)
            cout << endl;
    }
}