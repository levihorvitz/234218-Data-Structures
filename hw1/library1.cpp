//
// Created by 97250 on 18/04/2022.
//

#include"library1.h"
#include"competition.h"

void* Init()
{
    Competition* comp;
    try{
        comp = new Competition();
    }catch(std::bad_alloc&){
        comp = nullptr;
    }
    return (void*) comp;
}

StatusType AddCompany(void* DS, int CompanyID, int Value)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || CompanyID <= 0 || Value <= 0){
        return INVALID_INPUT;
    }
    try{
        comp->addGroup(CompanyID, Value);
    }catch(NodeAlreadyExists&){
        return FAILURE;
    }catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType AddEmployee(void* DS, int EmployeeID, int CompanyID, int Salary, int Grade)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || CompanyID <= 0 || EmployeeID <= 0 || Grade < 0 || Salary <= 0){
        return INVALID_INPUT;
    }
    try{
        comp->addPlayer(EmployeeID, CompanyID, Grade, Salary);
    }catch(NodeAlreadyExists&){
        return FAILURE;
    }catch(NodeNotExists&){
        return FAILURE;
    }catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType RemoveCompany(void* DS, int CompanyID)
{
    Competition* comp = (Competition*) DS;
	if (comp == nullptr || CompanyID <= 0){
        return INVALID_INPUT;
    }
	try{
	    comp->removeGroup(CompanyID);
	}catch(NodeNotExists&){
	    return FAILURE;
	}catch(GroupNotEmpty&){
	    return FAILURE;
	}
	return SUCCESS;
}

StatusType RemoveEmployee(void* DS, int EmployeeID) {
    Competition *comp = (Competition *) DS;
    if (comp == nullptr || EmployeeID <= 0) {
        return INVALID_INPUT;
    }
    try {
        comp->removePlayer(EmployeeID);
    } catch (NodeNotExists &) {
        return FAILURE;
    } catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetCompanyInfo(void* DS, int CompanyID, int* Value, int* NumEmployees)
{
    Competition *comp = (Competition *) DS;
    if (comp == nullptr || Value == nullptr || NumEmployees == nullptr || CompanyID <= 0) {
        return INVALID_INPUT;
    }
    try{
        comp->getGroupInfo(CompanyID,Value,NumEmployees);
    }catch(NodeNotExists&){
        return FAILURE;
    }

    return SUCCESS;
}

StatusType GetEmployeeInfo(void* DS, int EmployeeID, int* EmployerID, int* Salary, int* Grade)
{
    Competition *comp = (Competition *) DS;
    if (comp == nullptr || EmployerID == nullptr || Salary == nullptr || Grade == nullptr || EmployeeID <= 0) {
        return INVALID_INPUT;
    }
    try{
        comp->getPlayerInfo(EmployeeID, EmployerID, Salary, Grade);
    }catch(NodeNotExists&){
        return FAILURE;
    }

    return SUCCESS;
}

StatusType IncreaseCompanyValue(void* DS, int CompanyID, int ValueIncrease)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || CompanyID <= 0 || ValueIncrease <= 0){
        return INVALID_INPUT;
    }
    try{
        comp->increaseGroupValue(CompanyID, ValueIncrease);
    }catch(NodeNotExists&){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType PromoteEmployee(void* DS, int EmployeeID, int SalaryIncrease, int BumpGrade)
{
    Competition *comp = (Competition *) DS;
    if (comp == nullptr || EmployeeID <= 0 || SalaryIncrease <= 0) {
        return INVALID_INPUT;
    }
    try{
        comp->increaseSalary(EmployeeID, SalaryIncrease, BumpGrade);
    }catch(NodeNotExists&){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType HireEmployee(void* DS, int EmployeeID, int NewCompanyID)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || EmployeeID <= 0 || NewCompanyID <= 0){
        return INVALID_INPUT;
    }
    try{
         comp->movePlayer(EmployeeID, NewCompanyID);
    }catch(NodeNotExists&){
        return FAILURE;
    }catch(SameGroup&){
        return FAILURE;
    }catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType AcquireCompany(void* DS, int AcquirerID, int TargetID, double Factor)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || AcquirerID <= 0 || TargetID <= 0 || AcquirerID == TargetID || Factor < 1.0){
        return INVALID_INPUT;
    }
    try{
        comp->replaceGroup(TargetID, AcquirerID, Factor);
    }catch(NodeNotExists&) {
        return FAILURE;
    }catch(Failure&){
        return FAILURE;
    }catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetHighestEarner(void* DS, int CompanyID, int* EmployeeID)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || CompanyID == 0 || EmployeeID == nullptr){
        return INVALID_INPUT;
    }
    try{
        *EmployeeID = comp->getHighestSalaryPlayerInGroup(CompanyID).getId();
    }catch(NodeNotExists&) {
        return FAILURE;
    }catch(TreeEmpty&){
        return FAILURE;
    }catch(std::bad_alloc&){ //not needed
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetAllEmployeesBySalary(void* DS, int CompanyID, int** Employees, int* NumOfEmployees)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || CompanyID == 0 || Employees == nullptr || NumOfEmployees == nullptr){
        return INVALID_INPUT;
    }

    try{
        *Employees = comp->getAllPlayersBySalary(CompanyID, NumOfEmployees);
    }catch(NodeNotExists&) {
        return FAILURE;
    }catch(TreeEmpty&){
        return FAILURE;
    }catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetHighestEarnerInEachCompany(void* DS, int NumOfCompanies, int** Employees)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || NumOfCompanies < 1 || Employees == nullptr){
        return INVALID_INPUT;
    }
    try{
        *Employees = comp->GetGroupsHighestSalary(NumOfCompanies);
    }catch(Failure&){
        return FAILURE;
    }catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType GetNumEmployeesMatching(void* DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId,
								   int MinSalary, int MinGrade, int* TotalNumOfEmployees, int* NumOfEmployees)
{
    Competition* comp = (Competition*) DS;
    if (comp == nullptr || TotalNumOfEmployees == nullptr || NumOfEmployees == nullptr ||
	        CompanyID == 0 || MinEmployeeID < 0 || MaxEmployeeId < 0 || MinSalary < 0 ||
	        MinGrade < 0 || MinEmployeeID > MaxEmployeeId){
	    return INVALID_INPUT;
	}
	try{
	    comp->getNumPlayersMatching(CompanyID, MinEmployeeID, MaxEmployeeId,
                MinSalary, MinGrade, TotalNumOfEmployees, NumOfEmployees);
	}catch(NodeNotExists&){
	    return FAILURE;
	}catch(TreeEmpty&){
	    return FAILURE;
	}catch(std::bad_alloc&){
	    return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

void Quit(void** DS)
{
    Competition* comp = (Competition*) (*DS);
    delete comp;
    *DS = nullptr;
}