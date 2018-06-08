{
  "targets": [
    {
      "target_name": "sentenceComparatorModule",
      "sources": [
        "sentenceComparatorModule.cc"  
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"] 
    }
  ]
}