#include <bits/stdc++.h>
using namespace std;

// ── 1. BULK I/O ── fread/fwrite eliminates per-char syscall overhead ───────────
static char ibuf[1<<23], obuf[1<<23];
static int  ipos=0, olen=0;
void  ioInit() { fread(ibuf,1,sizeof ibuf,stdin); }
char  gc()     { return ibuf[ipos++]; }
int   ri()     { int x=0,c; while((c=gc())<'0'); do x=x*10+c-'0'; while((c=gc())>='0'); return x; }
void  flushOut(){ fwrite(obuf,1,olen,stdout); }
void  wi(int x){ char t[12]; int k=0; do{t[k++]='0'+x%10;x/=10;}while(x); while(k--) obuf[olen++]=t[k+1]; }
void  wc(char c){ obuf[olen++]=c; }
void  ws(const char* s){ while(*s) obuf[olen++]=*s++; }

// ── 2. WORD PACKING ── 5 chars into uint64; uint64 map >> string map ──────────
uint64_t rw(){
    uint64_t w=0; int c,i=0;
    while((c=(uint8_t)gc())<=' ');
    do{ w|=(uint64_t)c<<(i++<<3); } while((c=(uint8_t)gc())>' ');
    return w;
}

// ── 3. SORTING NETWORK ── 5 compare-swaps for 4 elements, beats sort() ────────
#define CS(a,b) if((a)>(b)) swap(a,b)
inline uint32_t pack4(char a,char b,char c,char d){
    CS(a,b); CS(c,d); CS(a,c); CS(b,d); CS(b,c);
    return (uint8_t)a|((uint32_t)(uint8_t)b<<8)|((uint32_t)(uint8_t)c<<16)|((uint32_t)(uint8_t)d<<24);
}
#undef CS
inline uint32_t last4sig(uint64_t w){
    return pack4((w>>8)&0xFF,(w>>16)&0xFF,(w>>24)&0xFF,(w>>32)&0xFF);
}
inline uint32_t sig4skip(uint64_t w, int skip){
    char ch[5]; for(int i=0;i<5;i++) ch[i]=(w>>(i<<3))&0xFF;
    char b[4]; int idx=0;
    for(int i=0;i<5;i++) if(i!=skip) b[idx++]=ch[i];
    return pack4(b[0],b[1],b[2],b[3]);
}

int main(){
    ioInit();
    int n=ri(), Q=ri();

    vector<uint64_t> words(n);
    for(int i=0;i<n;i++) words[i]=rw();

    // Build sig → indices
    unordered_map<uint32_t,vector<int>> sigMap;
    sigMap.reserve(n*2); sigMap.max_load_factor(0.5);
    for(int i=0;i<n;i++) sigMap[last4sig(words[i])].push_back(i);

    // Collect edges, build CSR ── cache-friendly adjacency beats vector<vector>
    vector<int> deg(n,0);
    vector<pair<int,int>> edges; edges.reserve(n*4);
    for(int j=0;j<n;j++){
        uint32_t seen[5]; int ns=0;
        for(int k=0;k<5;k++){
            uint32_t s=sig4skip(words[j],k);
            bool dup=false;
            for(int x=0;x<ns;x++) if(seen[x]==s){dup=true;break;}
            if(dup) continue;
            seen[ns++]=s;
            auto it=sigMap.find(s);
            if(it==sigMap.end()) continue;
            for(int i:it->second) if(i!=j){ edges.push_back({i,j}); deg[i]++; }
        }
    }
    sigMap.clear(); // free early

    // ── 4. CSR GRAPH ── flat arrays = sequential memory access in BFS ─────────
    vector<int> head(n+1,0);
    for(int i=0;i<n;i++) head[i+1]=head[i]+deg[i];
    vector<int> cur(head.begin(),head.begin()+n);
    vector<int> adj(edges.size());
    for(auto&[u,v]:edges) adj[cur[u]++]=v;
    edges.clear(); edges.shrink_to_fit();

    // Word → index (uint64 key >> string key)
    unordered_map<uint64_t,int> id;
    id.reserve(n*2); id.max_load_factor(0.5);
    for(int i=0;i<n;i++) id[words[i]]=i;

    // ── 5. READ ALL QUERIES, GROUP BY SOURCE ─────────────────────────────────
    //    Each unique source pays exactly ONE BFS — no re-runs, no cache map
    vector<int> qSrc(Q), qTgt(Q), qAns(Q,-1);
    unordered_map<int,vector<int>> bySource; bySource.reserve(Q);
    for(int i=0;i<Q;i++){
        qSrc[i]=id[rw()]; qTgt[i]=id[rw()];
        bySource[qSrc[i]].push_back(i);
    }

    // ── 6. GENERATION TRICK ── no O(n) memset between BFS runs ───────────────
    vector<int> dist(n), gen(n,0);
    vector<int> bfsQ(n);   // array queue — avoids std::queue overhead
    int curGen=0;

    for(auto&[src,qIdxs]:bySource){
        ++curGen;
        int h=0,t=0;
        gen[src]=curGen; dist[src]=0; bfsQ[t++]=src;
        while(h<t){
            int u=bfsQ[h++];
            // CSR range: head[u]..head[u+1] — sequential read, cache-hot
            for(int e=head[u];e<head[u+1];e++){
                int v=adj[e];
                if(gen[v]!=curGen){
                    gen[v]=curGen; dist[v]=dist[u]+1; bfsQ[t++]=v;
                }
            }
        }
        for(int qi:qIdxs)
            if(gen[qTgt[qi]]==curGen) qAns[qi]=dist[qTgt[qi]];
    }

    // ── 7. BUFFERED OUTPUT ── single fwrite >> many putchar/printf calls ──────
    for(int i=0;i<Q;i++){
        if(qAns[i]<0) ws("Impossible");
        else wi(qAns[i]);
        wc('\n');
    }
    flushOut();
}