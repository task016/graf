#include "Graph.h"
#include "Edge.h"
#include <iostream>
#include <queue>
using namespace std;

Graph::Graph(){
    start=nullptr;
}
void Graph::insertNode(int v){
    Node* novi=new Node(v);
    if(start==nullptr){
        start=novi;
    }
    else{
        Node* tmp=start;
        while(tmp->link!=nullptr){
            tmp=tmp->link;
        }
        tmp->link=novi;
    }
}
Node* Graph::findNode(int v){
    Node* tmp=start;
    while(tmp!=nullptr && tmp->value!=v){
        tmp=tmp->link;
    }
    return tmp;
}
void Graph::insertEdge(int s,int d){
    Node* st=findNode(s);
    Node* dt=findNode(d);
    if(st!=nullptr && dt!=nullptr){
        Edge *grana=new Edge(dt);
        grana->link=st->adj;
        st->adj=grana;
    }
    else{
        cout<<"Nemoguce je ubaciti granu, trazeni cvor ne postoji"<<endl;
    }
}
Edge* Graph::findEdge(int s,int d){
    Node* st=findNode(s);
    Node* dt=findNode(d);
    if(st!=nullptr && dt!= nullptr){
        Edge* grana=st->adj;
        while(grana!=nullptr && grana->dest!=dt){
            grana=grana->link;
        }
        return grana;
    }
     else{
        return nullptr;
    }
}
void Graph::print(){
    Node* cvor=start;
    while(cvor!=nullptr){
        cout<<cvor->value<<" -> ";
        Edge *grana=cvor->adj;
        while(grana!=nullptr){
            cout<<grana->dest->value<<" |";
            grana=grana->link;
        }
        cout<<'\n';
        cvor=cvor->link;
    }
}
void Graph::deleteEdge(int s,int d){
    Node* st=findNode(s);
    Edge* grana=findEdge(s,d);
    if(grana!=nullptr){
        Edge* tmp=st->adj;
        Edge* prev=nullptr;
        while(tmp!=grana){
            prev=tmp;
            tmp=tmp->link;
        }
        if(prev!=nullptr){
            prev->link=grana->link;
        }
        else{
            st->adj=grana->link;
        }
        delete grana;
    }
}
void Graph::deleteNode(int v){
    Node* cvor=findNode(v);
    if(cvor!=nullptr){
        Node* tmp=start;
        while(tmp!=nullptr){
            deleteEdge(tmp->value,cvor->value);
            deleteEdge(cvor->value,tmp->value);
            tmp=tmp->link;
        }
        tmp=start;
        if(tmp==cvor){
            start=cvor->link;
        }
        else{
            while(tmp->link!=cvor)
                tmp=tmp->link;
            tmp->link=cvor->link;
        }
        delete cvor;
    }
}
int Graph::breadthTrav(Node* poc){//0-neobidjen,1-cekanje,2-obidjen
    queue<Node*> q;
    int count;
    Node* cvor=start;
    while(cvor!=nullptr){
        cvor->status=0;
        cvor=cvor->link;
    }
    cvor=poc;
    cvor->status=1;
    q.push(cvor);
    Edge* e;
    while(!q.empty()){
        cvor=q.front();
        q.pop();
        //obilazak
        count++;
        cout<<cvor->value<<' ';
        cvor->status=2;
        e=cvor->adj;
        while(e!=nullptr){
            if(e->dest->status==0){
                e->dest->status=1;
                q.push(e->dest);
            }
            e=e->link;
        }
    }
    return count;
}
void Graph::depthTrav(Node* rt){
    if(rt!=nullptr){
        cout<<rt->value<<' ';
        rt->status=2;
        Edge* e=rt->adj;
        while(e!=nullptr){
            if(e->dest->status==0){
                depthTrav(e->dest);
            }
            e=e->link;
        }
    }
}
Node* Graph::getStart(){
    return start;
}
int Graph::ulStepen(Node* cvor){
    Node* tmp=start;
    int count=0;
    while(tmp!=nullptr){
        Edge *e=tmp->adj;
        while(e!=nullptr){
            if(e->dest==cvor)
                count++;
            e=e->link;
        }
        tmp=tmp->link;
    }
    return count;
}
int Graph::topological(){
    queue<Node*> q;
    int count=0;
    Node* cvor=start;
    while(cvor!=nullptr){
        cvor->status=ulStepen(cvor);
        if(cvor->status==0){
            cvor->in=true;
            q.push(cvor);
        }
        cvor=cvor->link;
    }
    while(!q.empty()){
        count++;
        cvor=q.front();
        q.pop();
        cout<<cvor->value<<' ';
        Edge *e=cvor->adj;
        while(e!=nullptr){
            e->dest->status--;
            e=e->link;
        }
        cvor=start;
        while(cvor!=nullptr){
            if(cvor->status==0 && cvor->in==false){
                cvor->in=true;
                q.push(cvor);
            }
            cvor=cvor->link;
        }

    }
    return count;

}
bool Graph::isConnected(){
    int cnt=0;
    Node* cvor=start;
    while(cvor!=nullptr){
        cnt++;
        cvor=cvor->link;
    }
    cvor=start;
    while(cvor!=nullptr){
        int broj=breadthTrav(cvor);
        cout<<'\n';
        if(broj!=cnt)
            return false;
        cvor=cvor->link;
    }
    return true;
}
bool Graph::seqExists(const char* seq){
    char c=seq[0];
    Node* n=findNode(c-48);
    if(n!=nullptr){
        c=seq[1];
        int i=1;
        while(c!='\0'){
            Edge* e=n->adj;
            while(e!=nullptr){
                if(e->dest->value==c-48){
                    n=e->dest;
                }
                e=e->link;
            }
            if(n->value!=c-48)
                return false;
            i++;
            c=seq[i];
        }
        return true;
    }
    else
    {
        return false;
    }
    
}
void Graph::printReach(Node* s,int n){
    if(n!=0){
        Edge *e=s->adj;
        while(e!=nullptr){
            cout<<e->dest->value<<' ';
            printReach(e->dest,n-1);
            e=e->link;
        }
    }
}
// void Graph::getReachable(Node* aerodrom,int time){
//     if(time>0){
//         Edge *let=aerodrom->adj;
//         while(let!=nullptr){
//             if(time-let->vreme>0){
//                 cout<<let->dest->value;
//                 getReachable(let->dest,time-(let->vreme+let->dest->tPresedanja));
//             }
//             let=let->link;
//         }
//     }
// }