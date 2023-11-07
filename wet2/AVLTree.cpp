#include"AVLTree.h"


//constructor for semifull tree
AVLTree::AVLTree(int height,int toDelete)
{
    this->head=fullTree(height,NULL);
    int TD = toDelete;
    semiFull(this->head,TD);
    this->size=pow(2,height+1)-1-toDelete;
}

//from full to semifull
void  AVLTree::semiFull(Node* head,int& toDelete)
{
    if(toDelete<=0||head==NULL)
    {
        return;
    }
    semiFull(head->right,toDelete);
    if (toDelete <=0)
    {
        head->height=max(getHeight(head->left),getHeight(head->right))+1;
        return;
    }
    if(head->right == NULL && head->left == NULL)
    {
        if(head->parent!=NULL)
        {
            if(head->parent->right==head)
            {
                head->parent->right=NULL;
            }
            else
            {
                head->parent->left=NULL;
            }
        }
        toDelete--;
        delete head;
        return;
    }
    semiFull(head->left,toDelete);
    head->height=max(getHeight(head->left),getHeight(head->right))+1;
}

// build a full tree of NULL
Node* AVLTree::fullTree(int height,Node* tree, Node* parent)
{
    if(height==-1)
    {
        return NULL;
    }
    tree = new Node(0,parent);
    tree->height=height;
    tree->left=fullTree(height-1,tree->left,tree);
    tree->right=fullTree(height-1,tree->right,tree);
    return tree;
}

//add function
void AVLTree::add(int addValue)
{
    if(this->head==NULL)
    {
        Node* nodeValue = new Node(addValue,NULL);
        this->head=nodeValue;
        size++;
        return;
    }
    Node* parent= binarySearch(this->head,addValue);
    if(parent->level == addValue)
    { 
        //exist
        return;
    }
    Node* nodeValue = new Node(addValue,parent);
    if(addValue < parent->level)
    {
        parent->left=nodeValue;
    }
    else
    {
        parent->right=nodeValue;
    }
    this->size++;
    AbalanceTree(nodeValue);
}

Node* AVLTree::binarySearch(Node* head, int addValue)
{
    if(head==NULL ||head->level == addValue)
    {
        return head;
    }
    if(head->level < addValue)
    {
        if (head->right==NULL)
            return head;
        return binarySearch(head->right,addValue);
    }
    if(head->left==NULL)
    {
        return head;
    }    
    return binarySearch(head->left,addValue);  
}

void  AVLTree::AbalanceTree(Node *current)
{
    if(current==NULL)
    {
        return;
    }
    while(current->parent != NULL)
    {
        Node* curParent=current->parent;
        if(curParent->height >= current->height+1)
        {
            return;
        }
        curParent->height=current->height + 1;
        int curParentBF=calculateBF(curParent);
        balanceTree(curParent,curParentBF);
        if(curParentBF==2||curParentBF==-2)
        {
            updateHead();
            return;
        }
        current=curParent;
    }
}

int AVLTree::calculateBF(Node* node) {
    if(node==NULL){
        return 0;
    }
    return getHeight(node->left)- getHeight(node->right);
}

int AVLTree::getHeight(Node* node) 
{
    if(node==NULL) 
    {
        return -1;
    }
    return node->height;
}

void  AVLTree::balanceTree(Node *current, int curBF)
{
    if(curBF==2)
    {
        int BFleft=calculateBF(current->left);
        if(BFleft>=0)
        {
            //LL
            rotateRight(current);
        }
        if(BFleft==-1)
        {
            //LR
            rotateLeft(current->left);
            rotateRight(current);
        }
    }
    else if(curBF==-2)
    {
        int BFright=calculateBF(current->right);
        if(BFright<=0)
        {
            //RR
            rotateLeft(current);
        }
        if(BFright==1)
        {
            //RL
            rotateRight(current->right);
            rotateLeft(current);
        }
    }
}

void AVLTree::
rotateRight(Node* damagedHead)
{
    if(!damagedHead)
    {
        return;
    }
    Node* headLeft = damagedHead->left;

    //for rank tree
    Node* damagedHeadRight = damagedHead->right;
    Node* headLeftLeft = headLeft->left;
    Node* headLeftRight = headLeft->right;
    //end od rank tree

    damagedHead->left = headLeft->right;
    headLeft->right = damagedHead;
    headLeft->parent = damagedHead->parent;
    if(damagedHead->left!=NULL){
        damagedHead->left->parent = damagedHead;
    }
    if(damagedHead->parent!=NULL)
    {
        if(damagedHead->parent->left==damagedHead)
        {
            damagedHead->parent->left=headLeft;
        }
        else
        {
            damagedHead->parent->right=headLeft;
        }

    }
    damagedHead->parent = headLeft;
    damagedHead->height = max(getHeight(damagedHead->left),getHeight(damagedHead->right))+1;
    headLeft->height = max(getHeight(headLeft->left),getHeight(headLeft->right))+1;
    updateRanks(damagedHead,damagedHeadRight,headLeft,headLeftLeft,headLeftRight);
}

void AVLTree::updateRanks(Node* damagedHead,Node* damagedHeadRight,Node* headLeft,Node* headLeftLeft,Node* headLeftRight)
{
    int damagedHeadRightAvg=0,headLeftRightAvg=0,headLeftLeftAvg=0;
    if(damagedHeadRight!=NULL)
    {
        damagedHeadRightAvg=damagedHeadRight->average;
    }
    if(headLeftRight!=NULL)
    {
        headLeftRightAvg=headLeftRight->average;
    }
    if(headLeftLeft!=NULL)
    {
        headLeftLeftAvg=headLeftLeft->average;
    }
    damagedHead->average = headLeftRightAvg + damagedHeadRightAvg + damagedHead->scores[0]*damagedHead->level;
    headLeft->average = damagedHead->average + headLeftLeftAvg + headLeft->scores[0]*headLeft->level;

    for(int i=0; i<201; i++)
    {
        int damagedHeadRightsum=0,headLeftRightsum=0,headLeftLeftsum=0;
        if(damagedHeadRight!=NULL)
        {
            damagedHeadRightsum=damagedHeadRight->sumScores[i];
        }
        if(headLeftRight!=NULL)
        {
            headLeftRightsum=headLeftRight->sumScores[i];
        }
        if(headLeftLeft!=NULL)
        {
            headLeftLeftsum=headLeftLeft->sumScores[i];
        }
        
        damagedHead->sumScores[i] = headLeftRightsum +  damagedHeadRightsum + damagedHead->scores[i];
        headLeft->sumScores[i] = damagedHead->sumScores[i] + headLeftLeftsum + headLeft->scores[i];
    }
}

void AVLTree::rotateLeft(Node* damagedHead)
{
    if(!damagedHead)
    {
        return;
    }
    Node* headRight = damagedHead->right;
    //for rank
    Node* damagedHeadLeft = damagedHead->left;
    Node* headRightRight = headRight->right;
    Node* headRightLeft = headRight->left;
    //end of rank
    
    damagedHead->right = headRight->left;
    headRight->left = damagedHead;
    headRight->parent = damagedHead->parent;
    if(damagedHead->right!=NULL){
        damagedHead->right->parent = damagedHead;
    }
    if(damagedHead->parent!=NULL)
    {
        if(damagedHead->parent->right==damagedHead)
        {
            damagedHead->parent->right=headRight;
        }
        else
        {
            damagedHead->parent->left=headRight;
        }

    }
    damagedHead->parent = headRight;
    damagedHead->height = max(getHeight(damagedHead->left),getHeight(damagedHead->right))+1;
    headRight->height = max(getHeight(headRight->left),getHeight(headRight->right))+1;
    
    updateRanks(damagedHead, headRightLeft, headRight,headRightRight, damagedHeadLeft);
}

void AVLTree::updateHead()
{
    if(this->head==NULL)
    {
        return;
    }
    while(this->head->parent!=NULL)
    {
        this->head=this->head->parent;
    }
}

//remove
void AVLTree::remove (int removeValue)
{
    // if not exist then do nothing
    if(this->head==NULL)
    {
        return;
    }
    Node* removeNode=binarySearch(this->head, removeValue);
    if(removeNode->level != removeValue)
    {
        //not exist
        return;
    }
    int sons = getSonsNum(removeNode);
    if (sons==0)
    {
        removeIfNoSons(removeNode);
    }
    if(sons==1) 
    {
        removeIfOneSon(removeNode);
    }
    if(sons==2) 
    {
        Node* toSwap=removeNode->right;
        while (toSwap->left)
        {
            toSwap=toSwap->left;
        }
        Node* start=toSwap->parent;
        while (start!=removeNode)
        {
            start->average-=toSwap->level*toSwap->scores[0];
            for (int i = 0; i < 201; i++)
            {
                start->sumScores[i]-=toSwap->scores[i];
            }
            start=start->parent;
        }

        int temp=removeNode->level;
        removeNode->level=toSwap->level;  
        toSwap->level=temp;
        for(int i=0; i<201; i++)
        {
            int temp2=removeNode->scores[i];
            removeNode->scores[i]=toSwap->scores[i];
            toSwap->scores[i]=temp2;
        }
        if(getSonsNum(toSwap)==0)
        {
            removeIfNoSons(toSwap);
        }
        else 
        {
            removeIfOneSon(toSwap);
        }
    }
    this->size--;
}

int AVLTree::getSonsNum(Node *head) 
{
    if(!head){
        return 0;
    }
    if(head->left==NULL && head->right==NULL){
        return 0;
    }
    if(head->left && head->right){
        return 2;
    }
    return 1;
}

void AVLTree::removeIfNoSons(Node* removeNode)
{
    if(removeNode==NULL)
    {
        return;
    }
    if(removeNode == this->head)
    {
        this->head = NULL;
        delete removeNode;
        return;
    }
    if(removeNode->parent!=NULL){
        if(removeNode->parent->left==removeNode){
            removeNode->parent->left=NULL;
        }
        if(removeNode->parent->right==removeNode)
        {
            removeNode->parent->right=NULL;
        }
        Node* parent= removeNode->parent;
        delete removeNode;
        RbalanceTree(parent);
    }
}

void  AVLTree::RbalanceTree(Node *current)
{
    if(current==NULL)
    {
        return;
    }
    while(current!=NULL)
    {
        current->height=max(getHeight(current->left),getHeight(current->right))+1;
        int curBF=calculateBF(current);
        balanceTree(current,curBF);
        current=current->parent;
    }
    updateHead();
}

void AVLTree::removeIfOneSon(Node *removeNode)
{
    if(removeNode==NULL)
    {
        return;
    }
    Node* parent=removeNode->parent;
    Node* son=removeNode->right;
    if(removeNode->right==NULL){
        son= removeNode->left;
    }
    if(removeNode==this->head){
        this->head=son;
        son->parent=NULL;
        delete removeNode;
        return;
    }
    if((parent->right!=NULL)&&(parent->right==removeNode)){
        parent->right=son;
    }
    if((parent->left!=NULL)&&(parent->left==removeNode)){
        parent->left=son;
    }
    son->parent=removeNode->parent;
    delete removeNode;
    RbalanceTree(parent);
}

AVLTree* AVLTree::mergedAVL(AVLTree* dis,AVLTree* src)
{    
    // we should not call this function with 2 empty groups
    if(dis==NULL||src==NULL)
    {
        return NULL;
    }
    if(src->size<=0)
    {
        Node* disArray = new Node[dis->size];
        int index=0;
        fillArray(disArray,dis->head,index,dis->size);
        int height = ceil(log2(dis->size+1))-1;
        AVLTree* semiFullTree=new AVLTree(height,pow(2,height+1)-1-dis->size);
        index = 0;
        fillSemiFull(semiFullTree->head,disArray,index,dis->size);
        calcuateNewRanks(semiFullTree->head);
        delete[] disArray;
        return semiFullTree;
    }
    if(dis->size<=0)
    {
        Node* srcArray = new Node[src->size];
        int index=0;
        fillArray(srcArray,src->head,index,src->size);
        int height = ceil(log2(src->size+1))-1;
        AVLTree* semiFullTree=new AVLTree(height,pow(2,height+1)-1-src->size);
        index = 0;
        fillSemiFull(semiFullTree->head,srcArray,index,src->size);
        calcuateNewRanks(semiFullTree->head);
        delete[] srcArray;
        return semiFullTree;
    }
    Node* disArray = new Node[dis->size];
    Node* srcArray = new Node [src->size]; 
    int newSize = dis->size+src->size;
    int index=0;
    fillArray(disArray,dis->head,index,dis->size);
    index=0;
    fillArray(srcArray,src->head,index, src->size);
    Node* mergedArray = new  Node[newSize];
    int final_size = mergeSort(mergedArray,disArray,dis->size,srcArray,src->size);
    int height = ceil(log2(final_size+1))-1;
    AVLTree* semiFullTree=new AVLTree(height,pow(2,height+1)-1-final_size);
    index=0;
    fillSemiFull(semiFullTree->head,mergedArray,index,final_size);
    calcuateNewRanks(semiFullTree->head);
    delete[] disArray;
    delete[] srcArray;
    delete[] mergedArray;
    return semiFullTree;
}

void AVLTree::fillArray(Node array[],Node* head,int& index, int size)
{
    if(index>=size || head == NULL)
    {
        return;
    }
    fillArray(array,head->left,index,size);
    if(index>=size)
    {
        return;
    }
    array[index++]=*head;
    fillArray(array,head->right,index,size);
}

int AVLTree::mergeSort(Node mergedArray[],Node array1[],int size1,Node array2[],int size2)
{
    int a1=0,a2=0,merged=0;
    while(a1 < size1 && a2 < size2)
    {
        if(array1[a1].level < array2[a2].level)
        {
            if (merged!=0 && array1[a1].level==mergedArray[merged-1].level)
            {
                for (int i = 0; i < 201; i++)
                {
                    mergedArray[merged-1].scores[i]+=array1[a1].scores[i];
                }
                a1++;
            }
            else
            {
                mergedArray[merged++]=array1[a1++];
            }
        }
        else
        {
            if (merged!=0 && array2[a2].level==mergedArray[merged-1].level)
            {
                for (int i = 0; i < 201; i++)
                {
                    mergedArray[merged-1].scores[i]+=array2[a2].scores[i];
                }
                a2++;
            }
            else
            {
                mergedArray[merged++]=array2[a2++];
            }
        }
    }
    while(a1 < size1)
    {
        if (merged!=0 && array1[a1].level==mergedArray[merged-1].level)
        {
            for (int i = 0; i < 201; i++)
            {
                mergedArray[merged-1].scores[i]+=array1[a1].scores[i];
            }
            a1++;
        }
        else
        {
            mergedArray[merged++]=array1[a1++];
        }
    }
    while(a2 < size2)
    {
        if (merged!=0 && array2[a2].level==mergedArray[merged-1].level)
        {
            for (int i = 0; i < 201; i++)
            {
                mergedArray[merged-1].scores[i]+=array2[a2].scores[i];
            }
            a2++;
        }
        else
        {
            mergedArray[merged++]=array2[a2++];
        }
    }
    return merged;
}

void AVLTree::fillSemiFull(Node* semiFullTree ,Node array[],int& index,int size)
{
    if(index>=size || semiFullTree==NULL)
    {
        return;
    }
    fillSemiFull(semiFullTree->left,array,index,size);
    if(index>=size)
    {
        return;
    }
    *semiFullTree=array[index++];
    fillSemiFull(semiFullTree->right,array,index,size);
}

AVLTree::~AVLTree()
{
    this->size=0;
    clearTree();
}

void AVLTree::clearTreeAux(Node* head)
{
    if(head==NULL)
        return;
    clearTreeAux(head->left);
    clearTreeAux(head->right);
    delete head;
}

void AVLTree::clearTree()
{
    Node* temp=this->head;
    this->head=NULL;
    clearTreeAux(temp);
    this->size=0;
}

bool AVLTree:: isExist(int level)
{
    Node* node=binarySearch(this->head,level);
    if(node==NULL)
    {
        return false;
    }
    if(node->level == level)
    {
        return true;
    }
    return false;
}

void AVLTree::inOrder(Node* head,int& size,int& index,int arr[])
{
    if(index>=size||head==NULL)
    {
        return;
    }
    inOrder(head->left,size,index,arr);
    if(index>=size)
    {
        return;
    }
    arr[index++]=head->level;
    inOrder(head->right,size,index,arr);
}

/*
//for debug purposes
template<class int,class compare>
void AVLTree::printDetails()
{
    printDetailsAux(this->head);
    std::cout<<"**************************\n";
}

template<class int,class compare>
void AVLTree::printDetailsAux(Node* head) 
{
    if(head == NULL)
    {
        return;
    }
    printDetailsAux(head->left);
    std::cout<<"VALUE: "<<head->level<<"  ";
    std::cout<<"BF: "<<calculateBF(head)<<"  ";
    std::cout<<"HEIGHT: "<<head->height<<"\n";
    printDetailsAux(head->right);
}*/

//for debug only

// void AVLTree::printPlayers(Node* head) 
// {
//     if(head == NULL)
//     {
//         //std::cout<<"NULL  ";   
//         return;
//     }
//     printPlayers(head->left);
//     // std::cout<<"level: "<<head->level<<"  ";
//     // std::cout<<"avg: "<<head->average<<"  ";
//     // std::cout<<"score[0]: "<<head->scores[0]<<"\n";
//     // std::cout<<"sumScores[0]: "<<head->sumScores[0]<<"\n";  
//     // std::cout<<"score[1]: "<<head->scores[1]<<"\n";
//     // std::cout<<"sumScores[1]: "<<head->sumScores[1]<<"\n"; 
//     // std::cout<<"*********************\n"; 
//     printPlayers(head->right);
// }


/*
void AVLTree::printGroup(Node* head, int &size) 
{
    if(head == NULL)
    {
        return;
    }
    printGroup(head->left,size);
    std::cout<<"VALUE: "<<head->level->groupID<<"  ";
    std::cout<<"BF: "<<calculateBF(head)<<"  ";
    std::cout<<"HEIGHT: "<<head->height<<"\n";
    
    size ++;
    if (head->level == NULL)
    {
       //std::cout<<"there is NULL\n";
    }
    printGroup(head->right,size);
}
*/

void AVLTree::addPlayer(int level,int score)
{
    if(!isExist(level))
    {
        add(level);
    }
    Node* level_node=binarySearch(this->head,level);
    //num of players in this level
    level_node->scores[0]++;
    level_node->scores[score]++;
    Node* parent=level_node;
    while (parent!=NULL)
    {
        parent->average+=level;
        parent->sumScores[0]++;
        parent->sumScores[score]++;
        parent=parent->parent;
    }
}

void AVLTree::updateRouteRank(Node* start, Node* removed)
{
    while (start!=NULL)
    {
        start->average-=removed->level;
        for (int i = 0; i < 201; i++)
        {
            start->sumScores[i]-=removed->scores[i];
        }
        start=start->parent;
    }
}

void AVLTree::removePlayer(int level, int score)
{
    if(!isExist(level))
    {
        return;
    }
    Node* level_node=binarySearch(this->head,level);
    //num of players in this level
    level_node->scores[0]--;
    level_node->scores[score]--;
    Node* parent=level_node;
    while (parent!=NULL)
    {
        parent->average-=level;
        parent->sumScores[0]--;
        parent->sumScores[score]--;
        parent=parent->parent;
    }
    if (level_node->scores[0]<=0)
    {
        remove(level);
    }
}

void AVLTree::calcuateNewRanks(Node* tree)
{
    if (tree==NULL)
    {
        return;
    }
    if (getSonsNum(tree)==0)
    {
        for (int i = 0; i < 201; i++)
        {
            tree->sumScores[i]=tree->scores[i];
            tree->average=tree->scores[0]*tree->level;
        }
        return;
    }    
    calcuateNewRanks(tree->left);
    calcuateNewRanks(tree->right);
    for (int i = 0; i < 201; i++)
    {
        int leftsum=0;
        int rightsum=0;
        int leftavg=0;
        int rightavg=0;
        if(tree->left!=NULL)
        {
            leftsum=tree->left->sumScores[i];
            leftavg=tree->left->average;
        }
        if(tree->right!=NULL)
        {
            rightsum=tree->right->sumScores[i];
            rightavg=tree->right->average;
        }
        tree->sumScores[i]=leftsum+rightsum+tree->scores[i];
        tree->average=tree->scores[0]*tree->level + leftavg + rightavg;
    }
}

int AVLTree::RankScore(int level, int score)
{
    int r=0;
    return RankScoreAux(this->head,level,r,score);
}
int AVLTree::RankScoreAux(Node* head,int level,int r,int score)
{
    if(head==NULL)
    {
        return 0;
    }
    if(head->level == level)
    {
        int rankLeft=0;
        if(head->left!=NULL)
        {
            rankLeft=head->left->sumScores[score];
        }
        return r+rankLeft+head->scores[score];
    }
    if(head->level < level)
    {
        int rankLeft=0;
        if(head->left!=NULL)
        {
            rankLeft=head->left->sumScores[score];
        }
        r=r+rankLeft+head->scores[score];
        return RankScoreAux(head->right,level,r,score);
    }
    return RankScoreAux(head->left,level,r,score);  
}

int AVLTree::select(Node* head,int m, int* players)
{
    if(head == NULL || m<=0)
    {
        return 0;
    }
    int left = 0;
    if(head->left!=NULL)
    {
        left=head->left->sumScores[0];
    }
    if(m > left && m <= left+head->scores[0])
    {
        *players=head->scores[0];
        return head->level;
    }
    if(left > m - head->scores[0])
    {
        return select(head->left,m,players);
    }
    return select(head->right, m-left-head->scores[0], players);
}

int AVLTree::RankAvg(int level)
{
    if (level == 0)
    {
        return 0;
    } 
    int r=0;
    return RankAvgAux(this->head,level,r);
}
int AVLTree::RankAvgAux(Node* head,int level,int r)
{
    if(head==NULL)
    {
        return 0;
    }
    if(head->level == level)
    {
        int rankLeft=0;
        if(head->left!=NULL)
        {
            rankLeft=head->left->average;
        }
        return r+rankLeft+(head->scores[0])*head->level;
    }
    if(head->level < level)
    {
        int rankLeft=0;
        if(head->left!=NULL)
        {
            rankLeft=head->left->average;
        }
        r=r+rankLeft+(head->scores[0])*head->level;
        return RankAvgAux(head->right,level,r);
    }
    return RankAvgAux(head->left,level,r);  
}

