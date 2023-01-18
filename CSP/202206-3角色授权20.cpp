#include <bits/stdc++.h>

using namespace std;
const int MAXN = 500;
const int MAXM = 500;
const int MAXQ = 5000;
struct User
{
    string name;
    vector<string> group;
};
struct Action
{
    string operation;
    string user;
    string resourceType;
    string resourceName;
    Action() {}
    Action(string operation, string user, string resourceType, string resourceName)
        : operation(operation), user(user), resourceType(resourceType), resourceName(resourceName) {}
};

struct Role
{
    string name;
    vector<string> operationNameList;
    vector<string> resourceTypeList;
    vector<string> resourceNameList;

    Role() {}
    Role(string name, vector<string> opL, vector<string> rtL, vector<string> rnL)
        : name(name), operationNameList(opL), resourceTypeList(rtL), resourceNameList(rnL) {}
};

struct RoleUser
{
    string roleName;
    vector<string> users;
    vector<string> groups;
    RoleUser() {}
    RoleUser(string roleName, vector<string> users, vector<string> groups)
        : roleName(roleName), users(users), groups(groups) {}
};

unordered_map<string, Role> roleTable;
unordered_map<string, RoleUser> roleUserTable;
unordered_map<string, User> userTable;
vector<Action> actions;

// 根据用户名和用户组查找用户的关联角色
// 查看用户关联角色的操作清单、资源种类清单和资源名称清单
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n, m, q;
    cin >> n >> m >> q;
    for (int i = 0; i < n; i++)
    {
        string roleName;
        int actionCount, resTypeCount, resNameCount;

        cin >> roleName >> actionCount;
        roleTable[roleName].name = roleName;
        for (int j = 0; j < actionCount; j++)
        {
            string opName;
            cin >> opName;
            roleTable[roleName].operationNameList.push_back(opName);
        }
        cin >> resTypeCount;
        for (int j = 0; j < resTypeCount; j++)
        {
            string resType;
            cin >> resType;
            roleTable[roleName].resourceTypeList.push_back(resType);
        }
        cin >> resNameCount;
        for (int j = 0; j < resNameCount; j++)
        {
            string resName;
            cin >> resName;
            roleTable[roleName].resourceNameList.push_back(resName);
        }
    }

    for (int i = 0; i < m; i++)
    {
        string roleName;
        int count;
        cin >> roleName >> count;
        roleUserTable[roleName].roleName = roleName;
        for (int j = 0; j < count; j++)
        {
            char type;
            string name;
            cin >> type >> name;
            if (type == 'u')
                roleUserTable[roleName].users.push_back(name);
            else
                roleUserTable[roleName].groups.push_back(name);
        }
    }

    for (int i = 0; i < q; i++)
    {
        string username;
        int groupCount;
        cin >> username >> groupCount;
        for (int j = 0; j < groupCount; j++)
        {
            string groupName;
            cin >> groupName;
            userTable[username].name = username;
            userTable[username].group.push_back(groupName);
        }
        string operation, resType, resName;
        cin >> operation >> resType >> resName;
        actions.push_back(Action(operation, username, resType, resName));
    }

    for (auto it = actions.begin(); it != actions.end(); it++)
    {
        set<string> authedRoles;
        // 执行当前操作的用户
        string cur_username = it->user;
        User cur_user = userTable[cur_username];
        // 当前操作的要用到的资源类型和资源名称
        string cur_operation = it->operation;
        string cur_resName = it->resourceName;
        string cur_resType = it->resourceType;
        // 查找当前用户的授权角色
        for (auto it = roleUserTable.begin(); it != roleUserTable.end(); it++)
        {
            vector<string> role_users = it->second.users;
            vector<string> role_groups = it->second.groups;
            if (find(role_users.begin(), role_users.end(), cur_username) != role_users.end())
                authedRoles.insert(it->second.roleName);
            for (auto it2 = cur_user.group.begin(); it2 != cur_user.group.end(); it2++)
            {
                if (find(role_groups.begin(), role_groups.end(), *it2) != role_groups.end())
                {
                    authedRoles.insert(it->second.roleName);
                }
            }
        }

        int flag = 0;
        // 查看当前用户的已授权角色资源列表是否允许当前操作
        for (auto it = authedRoles.begin(); it != authedRoles.end(); it++)
        {
            Role cur_role = roleTable[*it];
            if (find(cur_role.operationNameList.begin(), cur_role.operationNameList.end(), cur_operation) == cur_role.operationNameList.end() &&
                find(cur_role.operationNameList.begin(), cur_role.operationNameList.end(), "*") == cur_role.operationNameList.end())
                continue;
            else if (find(cur_role.resourceTypeList.begin(), cur_role.resourceTypeList.end(), cur_resType) == cur_role.resourceTypeList.end() &&
                     find(cur_role.resourceTypeList.begin(), cur_role.resourceTypeList.end(), "*") == cur_role.resourceTypeList.end())
                continue;
            else if (!cur_role.resourceNameList.empty() &&
                     find(cur_role.resourceNameList.begin(), cur_role.resourceNameList.end(), cur_resName) == cur_role.resourceNameList.end())
                continue;
            else
                flag = 1;
        }
        cout << flag;
        if (actions.end() - it > 1)
            cout << endl;
    }
    return 0;
}