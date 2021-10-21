import ROOT

energy_list=[0,14.41,39.52,81.00,121.78,122.06,136.47,200]
Ge_res={
energy_list[0]: [0.0979,0.1074,0.1288,0.1162,0.0984,0.1041],
energy_list[1]: [0.0979,0.1074,0.1288,0.1162,0.0984,0.1041],
energy_list[2]: [0.1357,0.1392,0.1592,0.1530,0.1373,0.1391],
energy_list[3]: [0.1721,0.1822,0.1986,0.1909,0.1751,0.1745],
energy_list[4]:[0.2115,0.2303,0.2334,0.2366,0.2094,0.2124],
energy_list[5]:[0.2115,0.2318,0.2300,0.2384,0.2093,0.2127],
energy_list[6]:[0.2214,0.2472,0.2439,0.2505,0.2215,0.2285]
}

_energy=[]
for iGe in range(6):
   _e=((Ge_res[energy_list[6]][iGe]-Ge_res[energy_list[5]][iGe])/(energy_list[6]-energy_list[5])*(energy_list[7]-energy_list[6]))+Ge_res[energy_list[6]][iGe]
   _energy.append(_e)
Ge_res.update({energy_list[7]:_energy})

if __name__=="__main__":
   fout=ROOT.TFile("./Ge_ResResponse.root","recreate")
   fout.cd()
   for i in range(6):
      gr = ROOT.TGraph() 
      gr.SetName("graph_Ge{}".format(i+1))
      for ie in range(len(energy_list)):
         gr.SetPoint(ie, energy_list[ie], Ge_res[energy_list[ie]][i])
      gr.Write()
      del gr
   fout.Write()
