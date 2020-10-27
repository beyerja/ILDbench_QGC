import FitSetting as FS
import SeparationCalculation as SC

default_settings = {
  "high" :
   {
    "rec": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          })
      },
    "oc": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          })
      },
    "jc": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          })
      },
    "bc": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          })
      },
    "sld": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.6,
            "f_s2" : 1.3
          })
      },
    "gen": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.0,
            "f_m2" : 0.98,
            "f_s1" : 0.9,
            "f_s2" : 1.2
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.9,
            "f_s2" : 1.3
          })
      }
    },
  "low" :
   {
    "rec": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          })
      },
    "oc": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.9,
            "f_m1" : 1.0,
            "f_m2" : 0.95,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.9,
            "f_m1" : 1.0,
            "f_m2" : 0.8,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          })
      },
    "jc": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          })
      },
    "bc": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.9,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.9,
            "f_m1" : 1.0,
            "f_m2" : 0.8,
            "f_s1" : 0.7,
            "f_s2" : 1.5
          })
      },
    "sld": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.8,
            "f_m1" : 1.0,
            "f_m2" : 1.0,
            "f_s1" : 0.5,
            "f_s2" : 1.5
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.1,
            "f_m2" : 0.9,
            "f_s1" : 0.5,
            "f_s2" : 2.0
          })
      },
    "gen": 
      { 
        "WW":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.0,
            "f_m2" : 0.98,
            "f_s1" : 0.9,
            "f_s2" : 1.2
          }), 
        "ZZ":
          FS.FitSetting(SC.DOUBLE_GAUSSIAN, 
            {"f1" : 0.7,
            "f_m1" : 1.0,
            "f_m2" : 0.98,
            "f_s1" : 0.9,
            "f_s2" : 1.2
          })
      }
    }
}
