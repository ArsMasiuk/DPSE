INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/coin

DEFINES += UNICODE HAVE_CMATH HAVE_FLOAT_H

SOURCES += \
#    $$PWD/src/coin/Clp/Clp_C_Interface.cpp \
#    $$PWD/src/coin/Clp/ClpCholeskyBase.cpp \
#    $$PWD/src/coin/Clp/ClpCholeskyDense.cpp \
#    $$PWD/src/coin/Clp/ClpCholeskyTaucs.cpp \
#    $$PWD/src/coin/Clp/ClpConstraint.cpp \
#    $$PWD/src/coin/Clp/ClpConstraintLinear.cpp \
#    $$PWD/src/coin/Clp/ClpConstraintQuadratic.cpp \
#    $$PWD/src/coin/Clp/ClpDualRowDantzig.cpp \
#    $$PWD/src/coin/Clp/ClpDualRowPivot.cpp \
#    $$PWD/src/coin/Clp/ClpDualRowSteepest.cpp \
#    $$PWD/src/coin/Clp/ClpDummyMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpDynamicExampleMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpDynamicMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpEventHandler.cpp \
#    $$PWD/src/coin/Clp/ClpFactorization.cpp \
#    $$PWD/src/coin/Clp/ClpGubDynamicMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpGubMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpHelperFunctions.cpp \
#    $$PWD/src/coin/Clp/ClpInterior.cpp \
#    $$PWD/src/coin/Clp/ClpLinearObjective.cpp \
#    $$PWD/src/coin/Clp/ClpLsqr.cpp \
#    $$PWD/src/coin/Clp/ClpMatrixBase.cpp \
#    $$PWD/src/coin/Clp/ClpMessage.cpp \
#    $$PWD/src/coin/Clp/ClpModel.cpp \
#    $$PWD/src/coin/Clp/ClpNetworkBasis.cpp \
#    $$PWD/src/coin/Clp/ClpNetworkMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpNode.cpp \
#    $$PWD/src/coin/Clp/ClpNonLinearCost.cpp \
#    $$PWD/src/coin/Clp/ClpObjective.cpp \
#    $$PWD/src/coin/Clp/ClpPackedMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpPdco.cpp \
#    $$PWD/src/coin/Clp/ClpPdcoBase.cpp \
#    $$PWD/src/coin/Clp/ClpPlusMinusOneMatrix.cpp \
#    $$PWD/src/coin/Clp/ClpPredictorCorrector.cpp \
#    $$PWD/src/coin/Clp/ClpPresolve.cpp \
#    $$PWD/src/coin/Clp/ClpPrimalColumnDantzig.cpp \
#    $$PWD/src/coin/Clp/ClpPrimalColumnPivot.cpp \
#    $$PWD/src/coin/Clp/ClpPrimalColumnSteepest.cpp \
#    $$PWD/src/coin/Clp/ClpQuadraticObjective.cpp \
#    $$PWD/src/coin/Clp/ClpSimplex.cpp \
#    $$PWD/src/coin/Clp/ClpSimplexDual.cpp \
#    $$PWD/src/coin/Clp/ClpSimplexNonlinear.cpp \
#    $$PWD/src/coin/Clp/ClpSimplexOther.cpp \
#    $$PWD/src/coin/Clp/ClpSimplexPrimal.cpp \
#    $$PWD/src/coin/Clp/ClpSolve.cpp \
#    $$PWD/src/coin/Clp/Idiot.cpp \
#    $$PWD/src/coin/Clp/IdiSolve.cpp \
#    $$PWD/src/coin/Clp/MyEventHandler.cpp \
#    $$PWD/src/coin/Clp/MyMessageHandler.cpp \
#    $$PWD/src/coin/Clp/unitTest.cpp \
#    $$PWD/src/coin/CoinUtils/CoinAlloc.cpp \
#    $$PWD/src/coin/CoinUtils/CoinBuild.cpp \
#    $$PWD/src/coin/CoinUtils/CoinDenseFactorization.cpp \
#    $$PWD/src/coin/CoinUtils/CoinDenseVector.cpp \
#    $$PWD/src/coin/CoinUtils/CoinError.cpp \
#    $$PWD/src/coin/CoinUtils/CoinFactorization1.cpp \
#    $$PWD/src/coin/CoinUtils/CoinFactorization2.cpp \
#    $$PWD/src/coin/CoinUtils/CoinFactorization3.cpp \
#    $$PWD/src/coin/CoinUtils/CoinFactorization4.cpp \
#    $$PWD/src/coin/CoinUtils/CoinFileIO.cpp \
#    $$PWD/src/coin/CoinUtils/CoinFinite.cpp \
#    $$PWD/src/coin/CoinUtils/CoinIndexedVector.cpp \
#    $$PWD/src/coin/CoinUtils/CoinLpIO.cpp \
#    $$PWD/src/coin/CoinUtils/CoinMessage.cpp \
#    $$PWD/src/coin/CoinUtils/CoinMessageHandler.cpp \
#    $$PWD/src/coin/CoinUtils/CoinModel.cpp \
#    $$PWD/src/coin/CoinUtils/CoinModelUseful.cpp \
#    $$PWD/src/coin/CoinUtils/CoinModelUseful2.cpp \
#    $$PWD/src/coin/CoinUtils/CoinMpsIO.cpp \
#    $$PWD/src/coin/CoinUtils/CoinOslFactorization.cpp \
#    $$PWD/src/coin/CoinUtils/CoinOslFactorization2.cpp \
#    $$PWD/src/coin/CoinUtils/CoinOslFactorization3.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPackedMatrix.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPackedVector.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPackedVectorBase.cpp \
#    $$PWD/src/coin/CoinUtils/CoinParam.cpp \
#    $$PWD/src/coin/CoinUtils/CoinParamUtils.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPostsolveMatrix.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPrePostsolveMatrix.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveDoubleton.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveDual.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveDupcol.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveEmpty.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveFixed.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveForcing.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveHelperFunctions.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveImpliedFree.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveIsolated.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveMatrix.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolvePsdebug.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveSingleton.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveSubst.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveTighten.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveTripleton.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveUseless.cpp \
#    $$PWD/src/coin/CoinUtils/CoinPresolveZeros.cpp \
#    $$PWD/src/coin/CoinUtils/CoinSearchTree.cpp \
#    $$PWD/src/coin/CoinUtils/CoinShallowPackedVector.cpp \
#    $$PWD/src/coin/CoinUtils/CoinSimpFactorization.cpp \
#    $$PWD/src/coin/CoinUtils/CoinSnapshot.cpp \
#    $$PWD/src/coin/CoinUtils/CoinStructuredModel.cpp \
#    $$PWD/src/coin/CoinUtils/CoinWarmStartBasis.cpp \
#    $$PWD/src/coin/CoinUtils/CoinWarmStartDual.cpp \
#    $$PWD/src/coin/CoinUtils/CoinWarmStartPrimalDual.cpp \
#    $$PWD/src/coin/CoinUtils/CoinWarmStartVector.cpp \
#    $$PWD/src/coin/Osi/OsiAuxInfo.cpp \
#    $$PWD/src/coin/Osi/OsiBranchingObject.cpp \
#    $$PWD/src/coin/Osi/OsiChooseVariable.cpp \
#    $$PWD/src/coin/Osi/OsiColCut.cpp \
##    $$PWD/src/coin/Osi/OsiCpxSolverInterface.cpp \
#    $$PWD/src/coin/Osi/OsiCut.cpp \
#    $$PWD/src/coin/Osi/OsiCuts.cpp \
##    $$PWD/src/coin/Osi/OsiGrbSolverInterface.cpp \
#    $$PWD/src/coin/Osi/OsiNames.cpp \
#    $$PWD/src/coin/Osi/OsiPresolve.cpp \
#    $$PWD/src/coin/Osi/OsiRowCut.cpp \
#    $$PWD/src/coin/Osi/OsiRowCutDebugger.cpp \
#    $$PWD/src/coin/Osi/OsiSolverBranch.cpp \
#    $$PWD/src/coin/Osi/OsiSolverInterface.cpp \
#    $$PWD/src/coin/OsiClp/OsiClpSolverInterface.cpp \
    $$PWD/src/ogdf/augmentation/DfsMakeBiconnected.cpp \
    $$PWD/src/ogdf/augmentation/PlanarAugmentation.cpp \
    $$PWD/src/ogdf/augmentation/PlanarAugmentationFix.cpp \
    $$PWD/src/ogdf/basic/graph_generators/randomHierarchy.cpp \
    $$PWD/src/ogdf/basic/internal/config.cpp \
    $$PWD/src/ogdf/basic/memory/PoolMemoryAllocator.cpp \
    $$PWD/src/ogdf/basic/AcyclicSubgraphModule.cpp \
    $$PWD/src/ogdf/basic/AdjacencyOracle.cpp \
    $$PWD/src/ogdf/basic/basic.cpp \
    $$PWD/src/ogdf/basic/CombinatorialEmbedding.cpp \
    $$PWD/src/ogdf/basic/DualGraph.cpp \
    $$PWD/src/ogdf/basic/EdgeComparer.cpp \
    $$PWD/src/ogdf/basic/EdgeComparerSimple.cpp \
    $$PWD/src/ogdf/basic/extended_graph_alg.cpp \
    $$PWD/src/ogdf/basic/filesystem.cpp \
    $$PWD/src/ogdf/basic/geometry.cpp \
    $$PWD/src/ogdf/basic/GF2Solver.cpp \
    $$PWD/src/ogdf/basic/Graph.cpp \
    $$PWD/src/ogdf/basic/graph_generators.cpp \
    $$PWD/src/ogdf/basic/GraphAttributes.cpp \
    $$PWD/src/ogdf/basic/GraphCopy.cpp \
    $$PWD/src/ogdf/basic/graphics.cpp \
    $$PWD/src/ogdf/basic/GridLayout.cpp \
    $$PWD/src/ogdf/basic/GridLayoutModule.cpp \
    $$PWD/src/ogdf/basic/Hashing.cpp \
    $$PWD/src/ogdf/basic/Layout.cpp \
    $$PWD/src/ogdf/basic/LayoutStandards.cpp \
    $$PWD/src/ogdf/basic/LayoutStatistics.cpp \
    $$PWD/src/ogdf/basic/LayoutStatistics_intersect.cpp \
    $$PWD/src/ogdf/basic/Logger.cpp \
    $$PWD/src/ogdf/basic/Math.cpp \
    $$PWD/src/ogdf/basic/Module.cpp \
    $$PWD/src/ogdf/basic/NearestRectangleFinder.cpp \
    $$PWD/src/ogdf/basic/PreprocessorLayout.cpp \
    $$PWD/src/ogdf/basic/simple_graph_alg.cpp \
    $$PWD/src/ogdf/basic/STNumbering.cpp \
    $$PWD/src/ogdf/basic/Stopwatch.cpp \
    $$PWD/src/ogdf/basic/System.cpp \
    $$PWD/src/ogdf/cluster/internal/ChunkConnection.cpp \
    $$PWD/src/ogdf/cluster/internal/ClusterKuratowskiConstraint.cpp \
    $$PWD/src/ogdf/cluster/internal/CP_MasterBase.cpp \
    $$PWD/src/ogdf/cluster/internal/CPlanarityMaster.cpp \
    $$PWD/src/ogdf/cluster/internal/CPlanaritySub.cpp \
    $$PWD/src/ogdf/cluster/internal/CPlanarSubClusteredST.cpp \
    $$PWD/src/ogdf/cluster/internal/CutConstraint.cpp \
    $$PWD/src/ogdf/cluster/internal/EdgeVar.cpp \
    $$PWD/src/ogdf/cluster/internal/MaxCPlanarMaster.cpp \
    $$PWD/src/ogdf/cluster/internal/MaxCPlanarSub.cpp \
    $$PWD/src/ogdf/cluster/internal/MaxPlanarEdgesConstraint.cpp \
    $$PWD/src/ogdf/cluster/internal/MinimalClusterConnection.cpp \
    $$PWD/src/ogdf/cluster/CconnectClusterPlanar.cpp \
    $$PWD/src/ogdf/cluster/CconnectClusterPlanarEmbed.cpp \
    $$PWD/src/ogdf/cluster/ClusterAnalysis.cpp \
    $$PWD/src/ogdf/cluster/ClusterGraph.cpp \
    $$PWD/src/ogdf/cluster/ClusterGraphAttributes.cpp \
    $$PWD/src/ogdf/cluster/ClusterOrthoLayout.cpp \
    $$PWD/src/ogdf/cluster/ClusterOrthoShaper.cpp \
    $$PWD/src/ogdf/cluster/ClusterPlanarity.cpp \
    $$PWD/src/ogdf/cluster/ClusterPlanarizationLayout.cpp \
    $$PWD/src/ogdf/cluster/ClusterPlanRep.cpp \
    $$PWD/src/ogdf/cluster/CPlanarEdgeInserter.cpp \
    $$PWD/src/ogdf/cluster/CPlanarSubClusteredGraph.cpp \
    $$PWD/src/ogdf/cluster/HananiTutteCPlanarity.cpp \
    $$PWD/src/ogdf/cluster/MaximumCPlanarSubgraph.cpp \
    $$PWD/src/ogdf/decomposition/BCTree.cpp \
    $$PWD/src/ogdf/decomposition/DynamicBCTree.cpp \
    $$PWD/src/ogdf/decomposition/DynamicSPQRForest.cpp \
    $$PWD/src/ogdf/decomposition/DynamicSPQRTree.cpp \
    $$PWD/src/ogdf/decomposition/PlanarSPQRTree.cpp \
    $$PWD/src/ogdf/decomposition/StaticSPQRTree.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/Attraction.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/EnergyFunction.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/NodePairEnergy.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/Overlap.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/Planarity.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/PlanarityGrid.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/Repulsion.cpp \
    $$PWD/src/ogdf/energybased/davidson_harel/UniformGrid.cpp \
    $$PWD/src/ogdf/energybased/dtree/GalaxyLevel.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/ArrayGraph.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/EdgeChain.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/FMEKernel.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/FMEMultipoleKernel.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/FMEThread.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/GalaxyMultilevel.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/LinearQuadtree.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/LinearQuadtreeBuilder.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/LinearQuadtreeExpansion.cpp \
    $$PWD/src/ogdf/energybased/fast_multipole_embedder/WSPD.cpp \
    $$PWD/src/ogdf/energybased/fmmm/new_multipole_method/QuadTreeNM.cpp \
    $$PWD/src/ogdf/energybased/fmmm/new_multipole_method/QuadTreeNodeNM.cpp \
    $$PWD/src/ogdf/energybased/fmmm/EdgeAttributes.cpp \
    $$PWD/src/ogdf/energybased/fmmm/FruchtermanReingold.cpp \
    $$PWD/src/ogdf/energybased/fmmm/MAARPacking.cpp \
    $$PWD/src/ogdf/energybased/fmmm/Multilevel.cpp \
    $$PWD/src/ogdf/energybased/fmmm/NewMultipoleMethod.cpp \
    $$PWD/src/ogdf/energybased/fmmm/NodeAttributes.cpp \
    $$PWD/src/ogdf/energybased/fmmm/numexcept.cpp \
    $$PWD/src/ogdf/energybased/fmmm/Set.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/BarycenterPlacer.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/CirclePlacer.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/EdgeCoverMerger.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/IndependentSetMerger.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/LocalBiconnectedMerger.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/MatchingMerger.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/MedianPlacer.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/ModularMultilevelMixer.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/MultilevelGraph.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/RandomMerger.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/RandomPlacer.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/ScalingLayout.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/SolarMerger.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/SolarPlacer.cpp \
    $$PWD/src/ogdf/energybased/multilevel_mixer/ZeroPlacer.cpp \
    $$PWD/src/ogdf/energybased/spring_embedder/SEGV_ForceModel.cpp \
    $$PWD/src/ogdf/energybased/DavidsonHarel.cpp \
    $$PWD/src/ogdf/energybased/DavidsonHarelLayout.cpp \
    $$PWD/src/ogdf/energybased/FastMultipoleEmbedder.cpp \
    $$PWD/src/ogdf/energybased/FMMMLayout.cpp \
    $$PWD/src/ogdf/energybased/GEMLayout.cpp \
    $$PWD/src/ogdf/energybased/MultilevelLayout.cpp \
    $$PWD/src/ogdf/energybased/PivotMDS.cpp \
    $$PWD/src/ogdf/energybased/SpringEmbedderFRExact.cpp \
    $$PWD/src/ogdf/energybased/SpringEmbedderGridVariant.cpp \
    $$PWD/src/ogdf/energybased/SpringEmbedderKK.cpp \
    $$PWD/src/ogdf/energybased/StressMinimization.cpp \
    $$PWD/src/ogdf/energybased/TutteLayout.cpp \
#    $$PWD/src/ogdf/external/backward.cpp \
#    $$PWD/src/ogdf/external/coin.cpp \
#    $$PWD/src/ogdf/external/Minisat.cpp \
    $$PWD/src/ogdf/fileformats/DLParser.cpp \
    $$PWD/src/ogdf/fileformats/DOT.cpp \
    $$PWD/src/ogdf/fileformats/DotLexer.cpp \
    $$PWD/src/ogdf/fileformats/DotParser.cpp \
    $$PWD/src/ogdf/fileformats/GDF.cpp \
    $$PWD/src/ogdf/fileformats/GdfParser.cpp \
    $$PWD/src/ogdf/fileformats/GEXF.cpp \
    $$PWD/src/ogdf/fileformats/GexfParser.cpp \
    $$PWD/src/ogdf/fileformats/GmlParser.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_dl.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_dot.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_gdf.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_gexf.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_gml.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_graphml.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_hyper.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_leda.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_ogml.cpp \
    $$PWD/src/ogdf/fileformats/GraphIO_tlp.cpp \
    $$PWD/src/ogdf/fileformats/GraphML.cpp \
    $$PWD/src/ogdf/fileformats/GraphMLParser.cpp \
    $$PWD/src/ogdf/fileformats/LineBuffer.cpp \
    $$PWD/src/ogdf/fileformats/Ogml.cpp \
    $$PWD/src/ogdf/fileformats/OgmlParser.cpp \
    $$PWD/src/ogdf/fileformats/SvgPrinter.cpp \
    $$PWD/src/ogdf/fileformats/Tlp.cpp \
    $$PWD/src/ogdf/fileformats/TlpLexer.cpp \
    $$PWD/src/ogdf/fileformats/TlpParser.cpp \
    $$PWD/src/ogdf/fileformats/UmlDiagramGraph.cpp \
    $$PWD/src/ogdf/fileformats/UmlModelGraph.cpp \
    $$PWD/src/ogdf/fileformats/UmlToGraphConverter.cpp \
    $$PWD/src/ogdf/fileformats/Utils.cpp \
    $$PWD/src/ogdf/fileformats/XmlParser.cpp \
    $$PWD/src/ogdf/fileformats/XmlScanner.cpp \
    $$PWD/src/ogdf/graphalg/CliqueFinder.cpp \
    $$PWD/src/ogdf/graphalg/Clusterer.cpp \
    $$PWD/src/ogdf/graphalg/ConnectivityTester.cpp \
    $$PWD/src/ogdf/graphalg/ConvexHull.cpp \
    $$PWD/src/ogdf/graphalg/GraphReduction.cpp \
    $$PWD/src/ogdf/graphalg/MaxAdjOrdering.cpp \
    $$PWD/src/ogdf/graphalg/MinimumCut.cpp \
    $$PWD/src/ogdf/graphalg/ModifiedNibbleClusterer.cpp \
    $$PWD/src/ogdf/graphalg/PageRank.cpp \
    $$PWD/src/ogdf/graphalg/ShortestPathWithBFM.cpp \
    $$PWD/src/ogdf/graphalg/Triconnectivity.cpp \
    $$PWD/src/ogdf/hypergraph/EdgeStandardRep.cpp \
    $$PWD/src/ogdf/hypergraph/Hypergraph.cpp \
    $$PWD/src/ogdf/hypergraph/HypergraphLayout.cpp \
    $$PWD/src/ogdf/labeling/ELabelPosSimple.cpp \
    $$PWD/src/ogdf/layered/BarycenterHeuristic.cpp \
    $$PWD/src/ogdf/layered/BlockOrder.cpp \
    $$PWD/src/ogdf/layered/CoffmanGrahamRanking.cpp \
    $$PWD/src/ogdf/layered/CrossingMinInterfaces.cpp \
    $$PWD/src/ogdf/layered/CrossingsMatrix.cpp \
    $$PWD/src/ogdf/layered/DfsAcyclicSubgraph.cpp \
    $$PWD/src/ogdf/layered/FastHierarchyLayout.cpp \
    $$PWD/src/ogdf/layered/FastSimpleHierarchyLayout.cpp \
    $$PWD/src/ogdf/layered/GreedyCycleRemoval.cpp \
    $$PWD/src/ogdf/layered/GreedyInsertHeuristic.cpp \
    $$PWD/src/ogdf/layered/GreedySwitchHeuristic.cpp \
    $$PWD/src/ogdf/layered/HierarchyLayoutModule.cpp \
    $$PWD/src/ogdf/layered/LongestPathRanking.cpp \
    $$PWD/src/ogdf/layered/MedianHeuristic.cpp \
    $$PWD/src/ogdf/layered/OptimalHierarchyClusterLayout.cpp \
    $$PWD/src/ogdf/layered/OptimalHierarchyLayout.cpp \
    $$PWD/src/ogdf/layered/OptimalRanking.cpp \
    $$PWD/src/ogdf/layered/SiftingHeuristic.cpp \
    $$PWD/src/ogdf/layered/SplitHeuristic.cpp \
    $$PWD/src/ogdf/layered/SugiyamaLayout-cluster.cpp \
    $$PWD/src/ogdf/layered/SugiyamaLayout.cpp \
#    $$PWD/src/ogdf/lib/abacus/aba_constraint.cpp \
#    $$PWD/src/ogdf/lib/abacus/abacusroot.cpp \
#    $$PWD/src/ogdf/lib/abacus/branchrule.cpp \
#    $$PWD/src/ogdf/lib/abacus/column.cpp \
#    $$PWD/src/ogdf/lib/abacus/conbranchrule.cpp \
#    $$PWD/src/ogdf/lib/abacus/conclass.cpp \
#    $$PWD/src/ogdf/lib/abacus/convar.cpp \
#    $$PWD/src/ogdf/lib/abacus/csense.cpp \
#    $$PWD/src/ogdf/lib/abacus/fixcand.cpp \
#    $$PWD/src/ogdf/lib/abacus/fsvarstat.cpp \
#    $$PWD/src/ogdf/lib/abacus/global.cpp \
#    $$PWD/src/ogdf/lib/abacus/history.cpp \
#    $$PWD/src/ogdf/lib/abacus/infeascon.cpp \
#    $$PWD/src/ogdf/lib/abacus/lp.cpp \
#    $$PWD/src/ogdf/lib/abacus/lpif.cpp \
#    $$PWD/src/ogdf/lib/abacus/lpmasterosi.cpp \
#    $$PWD/src/ogdf/lib/abacus/lpsub.cpp \
#    $$PWD/src/ogdf/lib/abacus/lpvarstat.cpp \
#    $$PWD/src/ogdf/lib/abacus/master.cpp \
#    $$PWD/src/ogdf/lib/abacus/opensub.cpp \
#    $$PWD/src/ogdf/lib/abacus/optsense.cpp \
#    $$PWD/src/ogdf/lib/abacus/osiif.cpp \
#    $$PWD/src/ogdf/lib/abacus/row.cpp \
#    $$PWD/src/ogdf/lib/abacus/setbranchrule.cpp \
#    $$PWD/src/ogdf/lib/abacus/slackstat.cpp \
#    $$PWD/src/ogdf/lib/abacus/sparvec.cpp \
#    $$PWD/src/ogdf/lib/abacus/srowcon.cpp \
#    $$PWD/src/ogdf/lib/abacus/sub.cpp \
#    $$PWD/src/ogdf/lib/abacus/tailoff.cpp \
#    $$PWD/src/ogdf/lib/abacus/valbranchrule.cpp \
#    $$PWD/src/ogdf/lib/abacus/variable.cpp \
#    $$PWD/src/ogdf/lib/abacus/vartype.cpp \
#    $$PWD/src/ogdf/lib/minisat/core/Solver.cpp \
#    $$PWD/src/ogdf/lib/minisat/simp/SimpSolver.cpp \
    $$PWD/src/ogdf/lib/pugixml/pugixml.cpp \
    $$PWD/src/ogdf/lpsolver/LPSolver.cpp \
    $$PWD/src/ogdf/misclayout/BalloonLayout.cpp \
    $$PWD/src/ogdf/misclayout/BertaultLayout.cpp \
    $$PWD/src/ogdf/misclayout/CircularLayout.cpp \
    $$PWD/src/ogdf/misclayout/LinearLayout.cpp \
    $$PWD/src/ogdf/misclayout/ProcrustesSubLayout.cpp \
    $$PWD/src/ogdf/orthogonal/edge_router/NodeInfo.cpp \
    $$PWD/src/ogdf/orthogonal/internal/CommonCompactionConstraintGraphBase.cpp \
    $$PWD/src/ogdf/orthogonal/CompactionConstraintGraph.cpp \
    $$PWD/src/ogdf/orthogonal/EdgeRouter.cpp \
    $$PWD/src/ogdf/orthogonal/FlowCompaction.cpp \
    $$PWD/src/ogdf/orthogonal/LongestPathCompaction.cpp \
    $$PWD/src/ogdf/orthogonal/OrthoLayout.cpp \
    $$PWD/src/ogdf/orthogonal/OrthoRep.cpp \
    $$PWD/src/ogdf/orthogonal/OrthoShaper.cpp \
    $$PWD/src/ogdf/packing/CCLayoutPackModule.cpp \
    $$PWD/src/ogdf/packing/ComponentSplitterLayout.cpp \
    $$PWD/src/ogdf/packing/SimpleCCPacker.cpp \
    $$PWD/src/ogdf/packing/TileToRowsCCPacker.cpp \
    $$PWD/src/ogdf/planarity/booth_lueker/EmbedPQTree.cpp \
    $$PWD/src/ogdf/planarity/booth_lueker/PlanarPQTree.cpp \
    $$PWD/src/ogdf/planarity/boyer_myrvold/BoyerMyrvoldInit.cpp \
    $$PWD/src/ogdf/planarity/boyer_myrvold/BoyerMyrvoldPlanar.cpp \
    $$PWD/src/ogdf/planarity/boyer_myrvold/FindKuratowskis.cpp \
    $$PWD/src/ogdf/planarity/embedder/CrossingStructure.cpp \
    $$PWD/src/ogdf/planarity/embedding_inserter/FixEdgeInserterCore.cpp \
    $$PWD/src/ogdf/planarity/embedding_inserter/VarEdgeInserterCore.cpp \
    $$PWD/src/ogdf/planarity/embedding_inserter/VarEdgeInserterDynCore.cpp \
    $$PWD/src/ogdf/planarity/planar_subgraph_fast/PlanarSubgraphPQTree.cpp \
    $$PWD/src/ogdf/planarity/planarization_layout/CliqueReplacer.cpp \
    $$PWD/src/ogdf/planarity/BoothLueker.cpp \
    $$PWD/src/ogdf/planarity/BoyerMyrvold.cpp \
    $$PWD/src/ogdf/planarity/EmbedderMaxFace.cpp \
    $$PWD/src/ogdf/planarity/EmbedderMaxFaceLayers.cpp \
    $$PWD/src/ogdf/planarity/EmbedderMinDepth.cpp \
    $$PWD/src/ogdf/planarity/EmbedderMinDepthMaxFace.cpp \
    $$PWD/src/ogdf/planarity/EmbedderMinDepthMaxFaceLayers.cpp \
    $$PWD/src/ogdf/planarity/EmbedderMinDepthPiTa.cpp \
    $$PWD/src/ogdf/planarity/EmbedderOptimalFlexDraw.cpp \
    $$PWD/src/ogdf/planarity/ExtractKuratowskis.cpp \
    $$PWD/src/ogdf/planarity/FixedEmbeddingInserter.cpp \
    $$PWD/src/ogdf/planarity/MaximumPlanarSubgraph.cpp \
    $$PWD/src/ogdf/planarity/MMCrossingMinimizationModule.cpp \
    $$PWD/src/ogdf/planarity/MMFixedEmbeddingInserter.cpp \
    $$PWD/src/ogdf/planarity/MMSubgraphPlanarizer.cpp \
    $$PWD/src/ogdf/planarity/MMVariableEmbeddingInserter.cpp \
    $$PWD/src/ogdf/planarity/MultiEdgeApproxInserter.cpp \
    $$PWD/src/ogdf/planarity/PlanarizationGridLayout.cpp \
    $$PWD/src/ogdf/planarity/PlanarizationLayout.cpp \
    $$PWD/src/ogdf/planarity/PlanarSubgraphBoyerMyrvold.cpp \
    $$PWD/src/ogdf/planarity/PlanRep.cpp \
    $$PWD/src/ogdf/planarity/PlanRepExpansion.cpp \
    $$PWD/src/ogdf/planarity/PlanRepInc.cpp \
    $$PWD/src/ogdf/planarity/PlanRepLight.cpp \
    $$PWD/src/ogdf/planarity/SimpleEmbedder.cpp \
    $$PWD/src/ogdf/planarity/SimpleIncNodeInserter.cpp \
    $$PWD/src/ogdf/planarity/SubgraphPlanarizer.cpp \
    $$PWD/src/ogdf/planarity/TopologyModule.cpp \
    $$PWD/src/ogdf/planarity/VariableEmbeddingInserter.cpp \
    $$PWD/src/ogdf/planarity/VariableEmbeddingInserterDyn.cpp \
    $$PWD/src/ogdf/planarlayout/mixed_model_layout/IOPoints.cpp \
    $$PWD/src/ogdf/planarlayout/mixed_model_layout/MixedModelBase.cpp \
    $$PWD/src/ogdf/planarlayout/BiconnectedShellingOrder.cpp \
    $$PWD/src/ogdf/planarlayout/BitonicOrdering.cpp \
    $$PWD/src/ogdf/planarlayout/FPPLayout.cpp \
    $$PWD/src/ogdf/planarlayout/LeftistOrdering.cpp \
    $$PWD/src/ogdf/planarlayout/MixedModelCrossingsBeautifierModule.cpp \
    $$PWD/src/ogdf/planarlayout/MixedModelLayout.cpp \
    $$PWD/src/ogdf/planarlayout/MMCBBase.cpp \
    $$PWD/src/ogdf/planarlayout/PlanarDrawLayout.cpp \
    $$PWD/src/ogdf/planarlayout/PlanarStraightLayout.cpp \
    $$PWD/src/ogdf/planarlayout/SchnyderLayout.cpp \
    $$PWD/src/ogdf/planarlayout/ShellingOrder.cpp \
    $$PWD/src/ogdf/planarlayout/ShellingOrderModule.cpp \
    $$PWD/src/ogdf/planarlayout/TriconnectedShellingOrder.cpp \
    $$PWD/src/ogdf/simultaneous/SimDraw.cpp \
    $$PWD/src/ogdf/simultaneous/SimDrawCaller.cpp \
    $$PWD/src/ogdf/simultaneous/SimDrawColorizer.cpp \
    $$PWD/src/ogdf/simultaneous/SimDrawCreator.cpp \
    $$PWD/src/ogdf/simultaneous/SimDrawCreatorSimple.cpp \
    $$PWD/src/ogdf/simultaneous/SimDrawManipulatorModule.cpp \
    $$PWD/src/ogdf/tree/LCA.cpp \
    $$PWD/src/ogdf/tree/RadialTreeLayout.cpp \
    $$PWD/src/ogdf/tree/TreeLayout.cpp \
    $$PWD/src/ogdf/uml/FixedEmbeddingInserterUML.cpp \
    $$PWD/src/ogdf/uml/OrthoLayoutUML.cpp \
    $$PWD/src/ogdf/uml/PlanarizationLayoutUML.cpp \
    $$PWD/src/ogdf/uml/PlanarizationLayoutUML_inc.cpp \
    $$PWD/src/ogdf/uml/PlanRepUML.cpp \
    $$PWD/src/ogdf/uml/SubgraphPlanarizerUML.cpp \
    $$PWD/src/ogdf/uml/UMLCrossingMinimizationModule.cpp \
    $$PWD/src/ogdf/uml/UMLGraph.cpp \
    $$PWD/src/ogdf/uml/VariableEmbeddingInserterDynUML.cpp \
    $$PWD/src/ogdf/uml/VariableEmbeddingInserterUML.cpp \
    $$PWD/src/ogdf/upward/internal/UpSAT.cpp \
    $$PWD/src/ogdf/upward/internal/UpwardPlanarityEmbeddedDigraph.cpp \
    $$PWD/src/ogdf/upward/internal/UpwardPlanaritySingleSource.cpp \
    $$PWD/src/ogdf/upward/DominanceLayout.cpp \
    $$PWD/src/ogdf/upward/ExpansionGraph.cpp \
    $$PWD/src/ogdf/upward/FaceSinkGraph.cpp \
    $$PWD/src/ogdf/upward/FixedEmbeddingUpwardEdgeInserter.cpp \
    $$PWD/src/ogdf/upward/FUPSSimple.cpp \
    $$PWD/src/ogdf/upward/LayerBasedUPRLayout.cpp \
    $$PWD/src/ogdf/upward/MaximalFUPS.cpp \
    $$PWD/src/ogdf/upward/SubgraphUpwardPlanarizer.cpp \
    $$PWD/src/ogdf/upward/UpwardPlanarity.cpp \
    $$PWD/src/ogdf/upward/UpwardPlanarSubgraphModule.cpp \
    $$PWD/src/ogdf/upward/UpwardPlanarSubgraphSimple.cpp \
    $$PWD/src/ogdf/upward/UpwardPlanRep.cpp \
    $$PWD/src/ogdf/upward/VisibilityLayout.cpp

DISTFILES += \
    $$PWD/include/ogdf/lib/abacus/bheap.inc \
    $$PWD/include/ogdf/lib/abacus/bprioqueue.inc \
    $$PWD/include/ogdf/lib/abacus/hash.inc \
    $$PWD/include/ogdf/lib/abacus/ring.inc

HEADERS += \
    $$PWD/include/coin/ClpCholeskyBase.hpp \
    $$PWD/include/coin/ClpCholeskyDense.hpp \
    $$PWD/include/coin/ClpCholeskyMumps.hpp \
    $$PWD/include/coin/ClpCholeskyTaucs.hpp \
    $$PWD/include/coin/ClpCholeskyUfl.hpp \
    $$PWD/include/coin/ClpCholeskyWssmp.hpp \
    $$PWD/include/coin/ClpCholeskyWssmpKKT.hpp \
    $$PWD/include/coin/ClpConfig.h \
    $$PWD/include/coin/ClpConstraint.hpp \
    $$PWD/include/coin/ClpConstraintLinear.hpp \
    $$PWD/include/coin/ClpConstraintQuadratic.hpp \
    $$PWD/include/coin/ClpDualRowDantzig.hpp \
    $$PWD/include/coin/ClpDualRowPivot.hpp \
    $$PWD/include/coin/ClpDualRowSteepest.hpp \
    $$PWD/include/coin/ClpDummyMatrix.hpp \
    $$PWD/include/coin/ClpDynamicExampleMatrix.hpp \
    $$PWD/include/coin/ClpDynamicMatrix.hpp \
    $$PWD/include/coin/ClpEventHandler.hpp \
    $$PWD/include/coin/ClpFactorization.hpp \
    $$PWD/include/coin/ClpGubDynamicMatrix.hpp \
    $$PWD/include/coin/ClpGubMatrix.hpp \
    $$PWD/include/coin/ClpHelperFunctions.hpp \
    $$PWD/include/coin/ClpInterior.hpp \
    $$PWD/include/coin/ClpLinearObjective.hpp \
    $$PWD/include/coin/ClpLsqr.hpp \
    $$PWD/include/coin/ClpMatrixBase.hpp \
    $$PWD/include/coin/ClpMessage.hpp \
    $$PWD/include/coin/ClpModel.hpp \
    $$PWD/include/coin/ClpNetworkBasis.hpp \
    $$PWD/include/coin/ClpNetworkMatrix.hpp \
    $$PWD/include/coin/ClpNode.hpp \
    $$PWD/include/coin/ClpNonLinearCost.hpp \
    $$PWD/include/coin/ClpObjective.hpp \
    $$PWD/include/coin/ClpPackedMatrix.hpp \
    $$PWD/include/coin/ClpParameters.hpp \
    $$PWD/include/coin/ClpPdco.hpp \
    $$PWD/include/coin/ClpPdcoBase.hpp \
    $$PWD/include/coin/ClpPlusMinusOneMatrix.hpp \
    $$PWD/include/coin/ClpPredictorCorrector.hpp \
    $$PWD/include/coin/ClpPresolve.hpp \
    $$PWD/include/coin/ClpPrimalColumnDantzig.hpp \
    $$PWD/include/coin/ClpPrimalColumnPivot.hpp \
    $$PWD/include/coin/ClpPrimalColumnSteepest.hpp \
    $$PWD/include/coin/ClpQuadraticObjective.hpp \
    $$PWD/include/coin/ClpSimplex.hpp \
    $$PWD/include/coin/ClpSimplexDual.hpp \
    $$PWD/include/coin/ClpSimplexNonlinear.hpp \
    $$PWD/include/coin/ClpSimplexOther.hpp \
    $$PWD/include/coin/ClpSimplexPrimal.hpp \
    $$PWD/include/coin/ClpSolve.hpp \
    $$PWD/include/coin/Coin_C_defines.h \
    $$PWD/include/coin/CoinAlloc.hpp \
    $$PWD/include/coin/CoinBuild.hpp \
    $$PWD/include/coin/CoinDenseFactorization.hpp \
    $$PWD/include/coin/CoinDenseVector.hpp \
    $$PWD/include/coin/CoinDistance.hpp \
    $$PWD/include/coin/CoinError.hpp \
    $$PWD/include/coin/CoinFactorization.hpp \
    $$PWD/include/coin/CoinFileIO.hpp \
    $$PWD/include/coin/CoinFinite.hpp \
    $$PWD/include/coin/CoinFloatEqual.hpp \
    $$PWD/include/coin/CoinHelperFunctions.hpp \
    $$PWD/include/coin/CoinIndexedVector.hpp \
    $$PWD/include/coin/CoinLpIO.hpp \
    $$PWD/include/coin/CoinMessage.hpp \
    $$PWD/include/coin/CoinMessageHandler.hpp \
    $$PWD/include/coin/CoinModel.hpp \
    $$PWD/include/coin/CoinModelUseful.hpp \
    $$PWD/include/coin/CoinMpsIO.hpp \
    $$PWD/include/coin/CoinOslC.h \
    $$PWD/include/coin/CoinOslFactorization.hpp \
    $$PWD/include/coin/CoinPackedMatrix.hpp \
    $$PWD/include/coin/CoinPackedVector.hpp \
    $$PWD/include/coin/CoinPackedVectorBase.hpp \
    $$PWD/include/coin/CoinParam.hpp \
    $$PWD/include/coin/CoinPragma.hpp \
    $$PWD/include/coin/CoinPresolveDoubleton.hpp \
    $$PWD/include/coin/CoinPresolveDual.hpp \
    $$PWD/include/coin/CoinPresolveDupcol.hpp \
    $$PWD/include/coin/CoinPresolveEmpty.hpp \
    $$PWD/include/coin/CoinPresolveFixed.hpp \
    $$PWD/include/coin/CoinPresolveForcing.hpp \
    $$PWD/include/coin/CoinPresolveImpliedFree.hpp \
    $$PWD/include/coin/CoinPresolveIsolated.hpp \
    $$PWD/include/coin/CoinPresolveMatrix.hpp \
    $$PWD/include/coin/CoinPresolvePsdebug.hpp \
    $$PWD/include/coin/CoinPresolveSingleton.hpp \
    $$PWD/include/coin/CoinPresolveSubst.hpp \
    $$PWD/include/coin/CoinPresolveTighten.hpp \
    $$PWD/include/coin/CoinPresolveTripleton.hpp \
    $$PWD/include/coin/CoinPresolveUseless.hpp \
    $$PWD/include/coin/CoinPresolveZeros.hpp \
    $$PWD/include/coin/CoinSearchTree.hpp \
    $$PWD/include/coin/CoinShallowPackedVector.hpp \
    $$PWD/include/coin/CoinSignal.hpp \
    $$PWD/include/coin/CoinSimpFactorization.hpp \
    $$PWD/include/coin/CoinSnapshot.hpp \
    $$PWD/include/coin/CoinSort.hpp \
    $$PWD/include/coin/CoinStructuredModel.hpp \
    $$PWD/include/coin/CoinTime.hpp \
    $$PWD/include/coin/CoinTypes.hpp \
    $$PWD/include/coin/CoinUtilsConfig.h \
    $$PWD/include/coin/CoinWarmStart.hpp \
    $$PWD/include/coin/CoinWarmStartBasis.hpp \
    $$PWD/include/coin/CoinWarmStartDual.hpp \
    $$PWD/include/coin/CoinWarmStartPrimalDual.hpp \
    $$PWD/include/coin/CoinWarmStartVector.hpp \
    $$PWD/include/coin/config.h \
    $$PWD/include/coin/config_clp_default.h \
    $$PWD/include/coin/config_coinutils_default.h \
    $$PWD/include/coin/config_default.h \
    $$PWD/include/coin/configall_system.h \
    $$PWD/include/coin/configall_system_msc.h \
    $$PWD/include/coin/Idiot.hpp \
    $$PWD/include/coin/MyEventHandler.hpp \
    $$PWD/include/coin/MyMessageHandler.hpp \
    $$PWD/include/coin/OsiAuxInfo.hpp \
    $$PWD/include/coin/OsiBranchingObject.hpp \
    $$PWD/include/coin/OsiChooseVariable.hpp \
    $$PWD/include/coin/OsiClpSolverInterface.hpp \
    $$PWD/include/coin/OsiColCut.hpp \
    $$PWD/include/coin/OsiCollections.hpp \
    $$PWD/include/coin/OsiCpxSolverInterface.hpp \
    $$PWD/include/coin/OsiCut.hpp \
    $$PWD/include/coin/OsiCuts.hpp \
    $$PWD/include/coin/OsiGrbSolverInterface.hpp \
    $$PWD/include/coin/OsiPresolve.hpp \
    $$PWD/include/coin/OsiRowCut.hpp \
    $$PWD/include/coin/OsiRowCutDebugger.hpp \
    $$PWD/include/coin/OsiSolverBranch.hpp \
    $$PWD/include/coin/OsiSolverInterface.hpp \
    $$PWD/include/coin/OsiSolverParameters.hpp \
    $$PWD/include/ogdf/augmentation/planar/PALabel.h \
    $$PWD/include/ogdf/augmentation/DfsMakeBiconnected.h \
    $$PWD/include/ogdf/augmentation/PlanarAugmentation.h \
    $$PWD/include/ogdf/augmentation/PlanarAugmentationFix.h \
    $$PWD/include/ogdf/basic/heap/BinaryHeap.h \
    $$PWD/include/ogdf/basic/heap/BinomialHeap.h \
    $$PWD/include/ogdf/basic/heap/FibonacciHeap.h \
    $$PWD/include/ogdf/basic/heap/HeapBase.h \
    $$PWD/include/ogdf/basic/heap/HotQueue.h \
    $$PWD/include/ogdf/basic/heap/PairingHeap.h \
    $$PWD/include/ogdf/basic/heap/RadixHeap.h \
    $$PWD/include/ogdf/basic/heap/RMHeap.h \
    $$PWD/include/ogdf/basic/internal/config.h \
    $$PWD/include/ogdf/basic/internal/config_autogen.h \
    $$PWD/include/ogdf/basic/internal/graph_iterators.h \
    $$PWD/include/ogdf/basic/internal/intrinsics.h \
    $$PWD/include/ogdf/basic/internal/list_templates.h \
    $$PWD/include/ogdf/basic/internal/version.h \
    $$PWD/include/ogdf/basic/memory/MallocMemoryAllocator.h \
    $$PWD/include/ogdf/basic/memory/PoolMemoryAllocator.h \
    $$PWD/include/ogdf/basic/pqtree/PQBasicKey.h \
    $$PWD/include/ogdf/basic/pqtree/PQBasicKeyRoot.h \
    $$PWD/include/ogdf/basic/pqtree/PQInternalKey.h \
    $$PWD/include/ogdf/basic/pqtree/PQInternalNode.h \
    $$PWD/include/ogdf/basic/pqtree/PQLeaf.h \
    $$PWD/include/ogdf/basic/pqtree/PQLeafKey.h \
    $$PWD/include/ogdf/basic/pqtree/PQNode.h \
    $$PWD/include/ogdf/basic/pqtree/PQNodeKey.h \
    $$PWD/include/ogdf/basic/pqtree/PQNodeRoot.h \
    $$PWD/include/ogdf/basic/AdjacencyOracle.h \
    $$PWD/include/ogdf/basic/AdjEntryArray.h \
    $$PWD/include/ogdf/basic/Array.h \
    $$PWD/include/ogdf/basic/Array2D.h \
    $$PWD/include/ogdf/basic/ArrayBuffer.h \
    $$PWD/include/ogdf/basic/Barrier.h \
    $$PWD/include/ogdf/basic/basic.h \
    $$PWD/include/ogdf/basic/BoundedQueue.h \
    $$PWD/include/ogdf/basic/CombinatorialEmbedding.h \
    $$PWD/include/ogdf/basic/comparer.h \
    $$PWD/include/ogdf/basic/DisjointSets.h \
    $$PWD/include/ogdf/basic/DualGraph.h \
    $$PWD/include/ogdf/basic/EdgeArray.h \
    $$PWD/include/ogdf/basic/EdgeComparer.h \
    $$PWD/include/ogdf/basic/EdgeComparerSimple.h \
    $$PWD/include/ogdf/basic/EpsilonTest.h \
    $$PWD/include/ogdf/basic/exceptions.h \
    $$PWD/include/ogdf/basic/extended_graph_alg.h \
    $$PWD/include/ogdf/basic/FaceArray.h \
    $$PWD/include/ogdf/basic/FaceSet.h \
    $$PWD/include/ogdf/basic/filesystem.h \
    $$PWD/include/ogdf/basic/geometry.h \
    $$PWD/include/ogdf/basic/GF2Solver.h \
    $$PWD/include/ogdf/basic/Graph.h \
    $$PWD/include/ogdf/basic/Graph_d.h \
    $$PWD/include/ogdf/basic/graph_generators.h \
    $$PWD/include/ogdf/basic/GraphAttributes.h \
    $$PWD/include/ogdf/basic/GraphCopy.h \
    $$PWD/include/ogdf/basic/GraphCopyAttributes.h \
    $$PWD/include/ogdf/basic/graphics.h \
    $$PWD/include/ogdf/basic/GraphList.h \
    $$PWD/include/ogdf/basic/GraphObserver.h \
    $$PWD/include/ogdf/basic/GridLayout.h \
    $$PWD/include/ogdf/basic/GridLayoutMapped.h \
    $$PWD/include/ogdf/basic/HashArray.h \
    $$PWD/include/ogdf/basic/HashArray2D.h \
    $$PWD/include/ogdf/basic/Hashing.h \
    $$PWD/include/ogdf/basic/HashIterator2D.h \
    $$PWD/include/ogdf/basic/IncNodeInserter.h \
    $$PWD/include/ogdf/basic/Layout.h \
    $$PWD/include/ogdf/basic/LayoutStandards.h \
    $$PWD/include/ogdf/basic/LayoutStatistics.h \
    $$PWD/include/ogdf/basic/List.h \
    $$PWD/include/ogdf/basic/Logger.h \
    $$PWD/include/ogdf/basic/Math.h \
    $$PWD/include/ogdf/basic/memory.h \
    $$PWD/include/ogdf/basic/MinHeap.h \
    $$PWD/include/ogdf/basic/Module.h \
    $$PWD/include/ogdf/basic/NearestRectangleFinder.h \
    $$PWD/include/ogdf/basic/NodeArray.h \
    $$PWD/include/ogdf/basic/NodeSet.h \
    $$PWD/include/ogdf/basic/PQTree.h \
    $$PWD/include/ogdf/basic/precondition.h \
    $$PWD/include/ogdf/basic/PreprocessorLayout.h \
    $$PWD/include/ogdf/basic/PriorityQueue.h \
    $$PWD/include/ogdf/basic/Queue.h \
    $$PWD/include/ogdf/basic/simple_graph_alg.h \
    $$PWD/include/ogdf/basic/Skiplist.h \
    $$PWD/include/ogdf/basic/SList.h \
    $$PWD/include/ogdf/basic/SortedSequence.h \
    $$PWD/include/ogdf/basic/STNumbering.h \
    $$PWD/include/ogdf/basic/Stopwatch.h \
    $$PWD/include/ogdf/basic/SubsetEnumerator.h \
    $$PWD/include/ogdf/basic/System.h \
    $$PWD/include/ogdf/basic/Thread.h \
    $$PWD/include/ogdf/basic/Timeouter.h \
    $$PWD/include/ogdf/basic/TopologyModule.h \
    $$PWD/include/ogdf/basic/tuples.h \
    $$PWD/include/ogdf/cluster/internal/basics.h \
    $$PWD/include/ogdf/cluster/internal/ChunkConnection.h \
    $$PWD/include/ogdf/cluster/internal/ClusterKuratowskiConstraint.h \
    $$PWD/include/ogdf/cluster/internal/ClusterPQContainer.h \
    $$PWD/include/ogdf/cluster/internal/CP_MasterBase.h \
    $$PWD/include/ogdf/cluster/internal/CPlanarEdgeVar.h \
    $$PWD/include/ogdf/cluster/internal/CPlanarityMaster.h \
    $$PWD/include/ogdf/cluster/internal/CPlanaritySub.h \
    $$PWD/include/ogdf/cluster/internal/CPlanarSubClusteredST.h \
    $$PWD/include/ogdf/cluster/internal/CutConstraint.h \
    $$PWD/include/ogdf/cluster/internal/EdgeVar.h \
    $$PWD/include/ogdf/cluster/internal/MaxCPlanarMaster.h \
    $$PWD/include/ogdf/cluster/internal/MaxCPlanarSub.h \
    $$PWD/include/ogdf/cluster/internal/MaxPlanarEdgesConstraint.h \
    $$PWD/include/ogdf/cluster/internal/MinimalClusterConnection.h \
    $$PWD/include/ogdf/cluster/CconnectClusterPlanar.h \
    $$PWD/include/ogdf/cluster/CconnectClusterPlanarEmbed.h \
    $$PWD/include/ogdf/cluster/ClusterAnalysis.h \
    $$PWD/include/ogdf/cluster/ClusterArray.h \
    $$PWD/include/ogdf/cluster/ClusterGraph.h \
    $$PWD/include/ogdf/cluster/ClusterGraphAttributes.h \
    $$PWD/include/ogdf/cluster/ClusterGraphCopyAttributes.h \
    $$PWD/include/ogdf/cluster/ClusterGraphObserver.h \
    $$PWD/include/ogdf/cluster/ClusterOrthoLayout.h \
    $$PWD/include/ogdf/cluster/ClusterOrthoShaper.h \
    $$PWD/include/ogdf/cluster/ClusterPlanarity.h \
    $$PWD/include/ogdf/cluster/ClusterPlanarizationLayout.h \
    $$PWD/include/ogdf/cluster/ClusterPlanRep.h \
    $$PWD/include/ogdf/cluster/ClusterSet.h \
    $$PWD/include/ogdf/cluster/CPlanarEdgeInserter.h \
    $$PWD/include/ogdf/cluster/CPlanarSubClusteredGraph.h \
    $$PWD/include/ogdf/cluster/HananiTutteCPlanarity.h \
    $$PWD/include/ogdf/cluster/MaximumCPlanarSubgraph.h \
    $$PWD/include/ogdf/decomposition/BCTree.h \
    $$PWD/include/ogdf/decomposition/DynamicBCTree.h \
    $$PWD/include/ogdf/decomposition/DynamicPlanarSPQRTree.h \
    $$PWD/include/ogdf/decomposition/DynamicSkeleton.h \
    $$PWD/include/ogdf/decomposition/DynamicSPQRForest.h \
    $$PWD/include/ogdf/decomposition/DynamicSPQRTree.h \
    $$PWD/include/ogdf/decomposition/PertinentGraph.h \
    $$PWD/include/ogdf/decomposition/PlanarSPQRTree.h \
    $$PWD/include/ogdf/decomposition/Skeleton.h \
    $$PWD/include/ogdf/decomposition/SPQRTree.h \
    $$PWD/include/ogdf/decomposition/StaticPlanarSPQRTree.h \
    $$PWD/include/ogdf/decomposition/StaticSkeleton.h \
    $$PWD/include/ogdf/decomposition/StaticSPQRTree.h \
    $$PWD/include/ogdf/energybased/davidson_harel/Attraction.h \
    $$PWD/include/ogdf/energybased/davidson_harel/EnergyFunction.h \
    $$PWD/include/ogdf/energybased/davidson_harel/NodePairEnergy.h \
    $$PWD/include/ogdf/energybased/davidson_harel/Overlap.h \
    $$PWD/include/ogdf/energybased/davidson_harel/Planarity.h \
    $$PWD/include/ogdf/energybased/davidson_harel/PlanarityGrid.h \
    $$PWD/include/ogdf/energybased/davidson_harel/Repulsion.h \
    $$PWD/include/ogdf/energybased/davidson_harel/UniformGrid.h \
    $$PWD/include/ogdf/energybased/dtree/DTree.h \
    $$PWD/include/ogdf/energybased/dtree/DTreeEmbedder.h \
    $$PWD/include/ogdf/energybased/dtree/DTreeForce.h \
    $$PWD/include/ogdf/energybased/dtree/DTreeForceTypes.h \
    $$PWD/include/ogdf/energybased/dtree/DTreeWSPD.h \
    $$PWD/include/ogdf/energybased/dtree/GalaxyLevel.h \
    $$PWD/include/ogdf/energybased/dtree/utils.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/ArrayGraph.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/ComplexDouble.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/EdgeChain.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/FastUtils.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/FMEFunc.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/FMEFunctional.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/FMEKernel.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/FMEMultipoleKernel.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/FMEThread.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/GalaxyMultilevel.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/LinearQuadtree.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/LinearQuadtreeBuilder.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/LinearQuadtreeExpansion.h \
    $$PWD/include/ogdf/energybased/fast_multipole_embedder/WSPD.h \
    $$PWD/include/ogdf/energybased/fmmm/maar_packing/PackingRowInfo.h \
    $$PWD/include/ogdf/energybased/fmmm/maar_packing/Rectangle.h \
    $$PWD/include/ogdf/energybased/fmmm/multilevel/Edge.h \
    $$PWD/include/ogdf/energybased/fmmm/multilevel/Node.h \
    $$PWD/include/ogdf/energybased/fmmm/new_multipole_method/ParticleInfo.h \
    $$PWD/include/ogdf/energybased/fmmm/new_multipole_method/QuadTreeNM.h \
    $$PWD/include/ogdf/energybased/fmmm/new_multipole_method/QuadTreeNodeNM.h \
    $$PWD/include/ogdf/energybased/fmmm/common.h \
    $$PWD/include/ogdf/energybased/fmmm/EdgeAttributes.h \
    $$PWD/include/ogdf/energybased/fmmm/FMMMOptions.h \
    $$PWD/include/ogdf/energybased/fmmm/FruchtermanReingold.h \
    $$PWD/include/ogdf/energybased/fmmm/MAARPacking.h \
    $$PWD/include/ogdf/energybased/fmmm/Multilevel.h \
    $$PWD/include/ogdf/energybased/fmmm/NewMultipoleMethod.h \
    $$PWD/include/ogdf/energybased/fmmm/NodeAttributes.h \
    $$PWD/include/ogdf/energybased/fmmm/numexcept.h \
    $$PWD/include/ogdf/energybased/fmmm/Set.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/BarycenterPlacer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/CirclePlacer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/EdgeCoverMerger.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/IndependentSetMerger.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/InitialPlacer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/LocalBiconnectedMerger.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/MatchingMerger.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/MedianPlacer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/ModularMultilevelMixer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/MultilevelBuilder.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/MultilevelGraph.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/RandomMerger.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/RandomPlacer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/ScalingLayout.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/SolarMerger.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/SolarPlacer.h \
    $$PWD/include/ogdf/energybased/multilevel_mixer/ZeroPlacer.h \
    $$PWD/include/ogdf/energybased/spring_embedder/common.h \
    $$PWD/include/ogdf/energybased/spring_embedder/MasterBase.h \
    $$PWD/include/ogdf/energybased/spring_embedder/SEGV_ForceModel.h \
    $$PWD/include/ogdf/energybased/spring_embedder/SpringEmbedderBase.h \
    $$PWD/include/ogdf/energybased/spring_embedder/WorkerBase.h \
    $$PWD/include/ogdf/energybased/DavidsonHarel.h \
    $$PWD/include/ogdf/energybased/DavidsonHarelLayout.h \
    $$PWD/include/ogdf/energybased/DTreeMultilevelEmbedder.h \
    $$PWD/include/ogdf/energybased/FastMultipoleEmbedder.h \
    $$PWD/include/ogdf/energybased/FMMMLayout.h \
    $$PWD/include/ogdf/energybased/GEMLayout.h \
    $$PWD/include/ogdf/energybased/MultilevelLayout.h \
    $$PWD/include/ogdf/energybased/PivotMDS.h \
    $$PWD/include/ogdf/energybased/SpringEmbedderFRExact.h \
    $$PWD/include/ogdf/energybased/SpringEmbedderGridVariant.h \
    $$PWD/include/ogdf/energybased/SpringEmbedderKK.h \
    $$PWD/include/ogdf/energybased/SpringForceModel.h \
    $$PWD/include/ogdf/energybased/StressMinimization.h \
    $$PWD/include/ogdf/energybased/TutteLayout.h \
    $$PWD/include/ogdf/external/abacus.h \
    $$PWD/include/ogdf/external/coin.h \
    $$PWD/include/ogdf/external/Minisat.h \
    $$PWD/include/ogdf/fileformats/DLParser.h \
    $$PWD/include/ogdf/fileformats/DOT.h \
    $$PWD/include/ogdf/fileformats/DotLexer.h \
    $$PWD/include/ogdf/fileformats/DotParser.h \
    $$PWD/include/ogdf/fileformats/GDF.h \
    $$PWD/include/ogdf/fileformats/GdfParser.h \
    $$PWD/include/ogdf/fileformats/GEXF.h \
    $$PWD/include/ogdf/fileformats/GexfParser.h \
    $$PWD/include/ogdf/fileformats/GmlParser.h \
    $$PWD/include/ogdf/fileformats/GraphIO.h \
    $$PWD/include/ogdf/fileformats/GraphML.h \
    $$PWD/include/ogdf/fileformats/GraphMLParser.h \
    $$PWD/include/ogdf/fileformats/LineBuffer.h \
    $$PWD/include/ogdf/fileformats/Ogml.h \
    $$PWD/include/ogdf/fileformats/OgmlParser.h \
    $$PWD/include/ogdf/fileformats/SvgPrinter.h \
    $$PWD/include/ogdf/fileformats/Tlp.h \
    $$PWD/include/ogdf/fileformats/TlpLexer.h \
    $$PWD/include/ogdf/fileformats/TlpParser.h \
    $$PWD/include/ogdf/fileformats/UmlDiagramGraph.h \
    $$PWD/include/ogdf/fileformats/UmlModelGraph.h \
    $$PWD/include/ogdf/fileformats/UmlToGraphConverter.h \
    $$PWD/include/ogdf/fileformats/Utils.h \
    $$PWD/include/ogdf/fileformats/XmlParser.h \
    $$PWD/include/ogdf/fileformats/XmlScanner.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/goemans/Approximation.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/goemans/BlowupComponents.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/goemans/BlowupGraph.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/goemans/CoreEdgeModule.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/goemans/CoreEdgeRandomSpanningTree.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/common_algorithms.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/EdgeWeightedGraph.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/EdgeWeightedGraphCopy.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/Full2ComponentGenerator.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/Full3ComponentGeneratorEnumeration.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/Full3ComponentGeneratorModule.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/Full3ComponentGeneratorVoronoi.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/FullComponentGeneratorDreyfusWagner.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/FullComponentStore.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/LPRelaxationSER.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/Save.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/SaveDynamic.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/SaveEnum.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/SaveStatic.h \
    $$PWD/include/ogdf/graphalg/steiner_tree/Triple.h \
    $$PWD/include/ogdf/graphalg/AStarSearch.h \
    $$PWD/include/ogdf/graphalg/CliqueFinder.h \
    $$PWD/include/ogdf/graphalg/Clusterer.h \
    $$PWD/include/ogdf/graphalg/ConnectivityTester.h \
    $$PWD/include/ogdf/graphalg/ConvexHull.h \
    $$PWD/include/ogdf/graphalg/Dijkstra.h \
    $$PWD/include/ogdf/graphalg/GraphReduction.h \
    $$PWD/include/ogdf/graphalg/MaxAdjOrdering.h \
    $$PWD/include/ogdf/graphalg/MaxFlowEdmondsKarp.h \
    $$PWD/include/ogdf/graphalg/MaxFlowGoldbergTarjan.h \
    $$PWD/include/ogdf/graphalg/MaxFlowSTPlanarDigraph.h \
    $$PWD/include/ogdf/graphalg/MaxFlowSTPlanarItaiShiloach.h \
    $$PWD/include/ogdf/graphalg/MinCostFlowReinelt.h \
    $$PWD/include/ogdf/graphalg/MinimumCut.h \
    $$PWD/include/ogdf/graphalg/MinSTCutBFS.h \
    $$PWD/include/ogdf/graphalg/MinSTCutDijkstra.h \
    $$PWD/include/ogdf/graphalg/MinSTCutMaxFlow.h \
    $$PWD/include/ogdf/graphalg/MinSTCutModule.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeDirectedCut.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeDualAscent.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeGoemans139.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeKou.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeMehlhorn.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreePrimalDual.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeRZLoss.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeShore.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeTakahashi.h \
    $$PWD/include/ogdf/graphalg/MinSteinerTreeZelikovsky.h \
    $$PWD/include/ogdf/graphalg/ModifiedNibbleClusterer.h \
    $$PWD/include/ogdf/graphalg/PageRank.h \
    $$PWD/include/ogdf/graphalg/ShortestPathAlgorithms.h \
    $$PWD/include/ogdf/graphalg/ShortestPathWithBFM.h \
    $$PWD/include/ogdf/graphalg/SteinerTreePreprocessing.h \
    $$PWD/include/ogdf/graphalg/Triconnectivity.h \
    $$PWD/include/ogdf/graphalg/Voronoi.h \
    $$PWD/include/ogdf/hypergraph/EdgeStandardRep.h \
    $$PWD/include/ogdf/hypergraph/Hypergraph.h \
    $$PWD/include/ogdf/hypergraph/HypergraphArray.h \
    $$PWD/include/ogdf/hypergraph/HypergraphAttributes.h \
    $$PWD/include/ogdf/hypergraph/HypergraphLayout.h \
    $$PWD/include/ogdf/hypergraph/HypergraphLayoutModule.h \
    $$PWD/include/ogdf/hypergraph/HypergraphObserver.h \
    $$PWD/include/ogdf/labeling/ELabelInterface.h \
    $$PWD/include/ogdf/labeling/ELabelPosSimple.h \
    $$PWD/include/ogdf/layered/BarycenterHeuristic.h \
    $$PWD/include/ogdf/layered/BlockOrder.h \
    $$PWD/include/ogdf/layered/CoffmanGrahamRanking.h \
    $$PWD/include/ogdf/layered/CrossingMinInterfaces.h \
    $$PWD/include/ogdf/layered/CrossingsMatrix.h \
    $$PWD/include/ogdf/layered/DfsAcyclicSubgraph.h \
    $$PWD/include/ogdf/layered/ExtendedNestingGraph.h \
    $$PWD/include/ogdf/layered/FastHierarchyLayout.h \
    $$PWD/include/ogdf/layered/FastSimpleHierarchyLayout.h \
    $$PWD/include/ogdf/layered/GreedyCycleRemoval.h \
    $$PWD/include/ogdf/layered/GreedyInsertHeuristic.h \
    $$PWD/include/ogdf/layered/GreedySwitchHeuristic.h \
    $$PWD/include/ogdf/layered/GridSifting.h \
    $$PWD/include/ogdf/layered/Hierarchy.h \
    $$PWD/include/ogdf/layered/HierarchyLevels.h \
    $$PWD/include/ogdf/layered/Level.h \
    $$PWD/include/ogdf/layered/LongestPathRanking.h \
    $$PWD/include/ogdf/layered/MedianHeuristic.h \
    $$PWD/include/ogdf/layered/OptimalHierarchyClusterLayout.h \
    $$PWD/include/ogdf/layered/OptimalHierarchyLayout.h \
    $$PWD/include/ogdf/layered/OptimalRanking.h \
    $$PWD/include/ogdf/layered/SiftingHeuristic.h \
    $$PWD/include/ogdf/layered/SplitHeuristic.h \
    $$PWD/include/ogdf/layered/SugiyamaLayout.h \
    $$PWD/include/ogdf/lib/abacus/abacusroot.h \
    $$PWD/include/ogdf/lib/abacus/active.h \
    $$PWD/include/ogdf/lib/abacus/active.inc \
    $$PWD/include/ogdf/lib/abacus/bheap.h \
    $$PWD/include/ogdf/lib/abacus/boundbranchrule.h \
    $$PWD/include/ogdf/lib/abacus/bprioqueue.h \
    $$PWD/include/ogdf/lib/abacus/branchrule.h \
    $$PWD/include/ogdf/lib/abacus/column.h \
    $$PWD/include/ogdf/lib/abacus/colvar.h \
    $$PWD/include/ogdf/lib/abacus/conbranchrule.h \
    $$PWD/include/ogdf/lib/abacus/conclass.h \
    $$PWD/include/ogdf/lib/abacus/constraint.h \
    $$PWD/include/ogdf/lib/abacus/convar.h \
    $$PWD/include/ogdf/lib/abacus/csense.h \
    $$PWD/include/ogdf/lib/abacus/cutbuffer.h \
    $$PWD/include/ogdf/lib/abacus/cutbuffer.inc \
    $$PWD/include/ogdf/lib/abacus/fixcand.h \
    $$PWD/include/ogdf/lib/abacus/fsvarstat.h \
    $$PWD/include/ogdf/lib/abacus/global.h \
    $$PWD/include/ogdf/lib/abacus/hash.h \
    $$PWD/include/ogdf/lib/abacus/history.h \
    $$PWD/include/ogdf/lib/abacus/infeascon.h \
    $$PWD/include/ogdf/lib/abacus/lp.h \
    $$PWD/include/ogdf/lib/abacus/lpmaster.h \
    $$PWD/include/ogdf/lib/abacus/lpmasterosi.h \
    $$PWD/include/ogdf/lib/abacus/lpsolution.h \
    $$PWD/include/ogdf/lib/abacus/lpsolution.inc \
    $$PWD/include/ogdf/lib/abacus/lpsub.h \
    $$PWD/include/ogdf/lib/abacus/lpsubosi.h \
    $$PWD/include/ogdf/lib/abacus/lpvarstat.h \
    $$PWD/include/ogdf/lib/abacus/master.h \
    $$PWD/include/ogdf/lib/abacus/nonduplpool.h \
    $$PWD/include/ogdf/lib/abacus/nonduplpool.inc \
    $$PWD/include/ogdf/lib/abacus/numcon.h \
    $$PWD/include/ogdf/lib/abacus/numvar.h \
    $$PWD/include/ogdf/lib/abacus/opensub.h \
    $$PWD/include/ogdf/lib/abacus/optsense.h \
    $$PWD/include/ogdf/lib/abacus/osiif.h \
    $$PWD/include/ogdf/lib/abacus/osiinclude.h \
    $$PWD/include/ogdf/lib/abacus/pool.h \
    $$PWD/include/ogdf/lib/abacus/poolslot.h \
    $$PWD/include/ogdf/lib/abacus/poolslot.inc \
    $$PWD/include/ogdf/lib/abacus/poolslotref.h \
    $$PWD/include/ogdf/lib/abacus/poolslotref.inc \
    $$PWD/include/ogdf/lib/abacus/ring.h \
    $$PWD/include/ogdf/lib/abacus/row.h \
    $$PWD/include/ogdf/lib/abacus/rowcon.h \
    $$PWD/include/ogdf/lib/abacus/separator.h \
    $$PWD/include/ogdf/lib/abacus/separator.inc \
    $$PWD/include/ogdf/lib/abacus/setbranchrule.h \
    $$PWD/include/ogdf/lib/abacus/slackstat.h \
    $$PWD/include/ogdf/lib/abacus/sparvec.h \
    $$PWD/include/ogdf/lib/abacus/srowcon.h \
    $$PWD/include/ogdf/lib/abacus/standardpool.h \
    $$PWD/include/ogdf/lib/abacus/standardpool.inc \
    $$PWD/include/ogdf/lib/abacus/sub.h \
    $$PWD/include/ogdf/lib/abacus/tailoff.h \
    $$PWD/include/ogdf/lib/abacus/valbranchrule.h \
    $$PWD/include/ogdf/lib/abacus/variable.h \
    $$PWD/include/ogdf/lib/abacus/vartype.h \
    $$PWD/include/ogdf/lib/backward/backward.hpp \
    $$PWD/include/ogdf/lib/minisat/core/Solver.h \
    $$PWD/include/ogdf/lib/minisat/core/SolverTypes.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Alg.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Alloc.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Heap.h \
    $$PWD/include/ogdf/lib/minisat/mtl/IntTypes.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Map.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Queue.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Sort.h \
    $$PWD/include/ogdf/lib/minisat/mtl/Vec.h \
    $$PWD/include/ogdf/lib/minisat/mtl/XAlloc.h \
    $$PWD/include/ogdf/lib/minisat/simp/SimpSolver.h \
    $$PWD/include/ogdf/lib/minisat/utils/Options.h \
    $$PWD/include/ogdf/lib/minisat/utils/ParseUtils.h \
    $$PWD/include/ogdf/lib/pugixml/pugiconfig.h \
    $$PWD/include/ogdf/lib/pugixml/pugixml.h \
    $$PWD/include/ogdf/lpsolver/LPSolver.h \
    $$PWD/include/ogdf/misclayout/BalloonLayout.h \
    $$PWD/include/ogdf/misclayout/BertaultLayout.h \
    $$PWD/include/ogdf/misclayout/CircularLayout.h \
    $$PWD/include/ogdf/misclayout/LinearLayout.h \
    $$PWD/include/ogdf/misclayout/ProcrustesSubLayout.h \
    $$PWD/include/ogdf/module/AcyclicSubgraphModule.h \
    $$PWD/include/ogdf/module/AugmentationModule.h \
    $$PWD/include/ogdf/module/CCLayoutPackModule.h \
    $$PWD/include/ogdf/module/ClustererModule.h \
    $$PWD/include/ogdf/module/ClusterPlanarModule.h \
    $$PWD/include/ogdf/module/CPlanarSubgraphModule.h \
    $$PWD/include/ogdf/module/CrossingMinimizationModule.h \
    $$PWD/include/ogdf/module/EdgeInsertionModule.h \
    $$PWD/include/ogdf/module/EmbedderModule.h \
    $$PWD/include/ogdf/module/ForceLayoutModule.h \
    $$PWD/include/ogdf/module/FUPSModule.h \
    $$PWD/include/ogdf/module/GridLayoutModule.h \
    $$PWD/include/ogdf/module/HierarchyClusterLayoutModule.h \
    $$PWD/include/ogdf/module/HierarchyLayoutModule.h \
    $$PWD/include/ogdf/module/LayerByLayerSweep.h \
    $$PWD/include/ogdf/module/LayeredCrossMinModule.h \
    $$PWD/include/ogdf/module/LayoutClusterPlanRepModule.h \
    $$PWD/include/ogdf/module/LayoutModule.h \
    $$PWD/include/ogdf/module/LayoutPlanRepModule.h \
    $$PWD/include/ogdf/module/LayoutPlanRepUMLModule.h \
    $$PWD/include/ogdf/module/MaxFlowModule.h \
    $$PWD/include/ogdf/module/MinCostFlowModule.h \
    $$PWD/include/ogdf/module/MinSteinerTreeModule.h \
    $$PWD/include/ogdf/module/MixedModelCrossingsBeautifierModule.h \
    $$PWD/include/ogdf/module/MMCrossingMinimizationModule.h \
    $$PWD/include/ogdf/module/MMEdgeInsertionModule.h \
    $$PWD/include/ogdf/module/MultilevelLayoutModule.h \
    $$PWD/include/ogdf/module/PlanarityModule.h \
    $$PWD/include/ogdf/module/PlanarSubgraphModule.h \
    $$PWD/include/ogdf/module/RankingModule.h \
    $$PWD/include/ogdf/module/ShellingOrderModule.h \
    $$PWD/include/ogdf/module/ShortestPathModule.h \
    $$PWD/include/ogdf/module/UMLCrossingMinimizationModule.h \
    $$PWD/include/ogdf/module/UMLEdgeInsertionModule.h \
    $$PWD/include/ogdf/module/UMLLayoutModule.h \
    $$PWD/include/ogdf/module/UPRLayoutModule.h \
    $$PWD/include/ogdf/module/UpwardEdgeInserterModule.h \
    $$PWD/include/ogdf/module/UpwardPlanarizerModule.h \
    $$PWD/include/ogdf/module/UpwardPlanarSubgraphModule.h \
    $$PWD/include/ogdf/orthogonal/edge_router/NodeInfo.h \
    $$PWD/include/ogdf/orthogonal/internal/CommonCompactionConstraintGraphBase.h \
    $$PWD/include/ogdf/orthogonal/internal/RoutingChannel.h \
    $$PWD/include/ogdf/orthogonal/CompactionConstraintGraph.h \
    $$PWD/include/ogdf/orthogonal/EdgeRouter.h \
    $$PWD/include/ogdf/orthogonal/FlowCompaction.h \
    $$PWD/include/ogdf/orthogonal/LongestPathCompaction.h \
    $$PWD/include/ogdf/orthogonal/MinimumEdgeDistances.h \
    $$PWD/include/ogdf/orthogonal/OrthoLayout.h \
    $$PWD/include/ogdf/orthogonal/OrthoRep.h \
    $$PWD/include/ogdf/orthogonal/OrthoShaper.h \
    $$PWD/include/ogdf/packing/ComponentSplitterLayout.h \
    $$PWD/include/ogdf/packing/SimpleCCPacker.h \
    $$PWD/include/ogdf/packing/TileToRowsCCPacker.h \
    $$PWD/include/ogdf/planarity/booth_lueker/EmbedIndicator.h \
    $$PWD/include/ogdf/planarity/booth_lueker/EmbedPQTree.h \
    $$PWD/include/ogdf/planarity/booth_lueker/IndInfo.h \
    $$PWD/include/ogdf/planarity/booth_lueker/PlanarLeafKey.h \
    $$PWD/include/ogdf/planarity/booth_lueker/PlanarPQTree.h \
    $$PWD/include/ogdf/planarity/boyer_myrvold/BoyerMyrvoldInit.h \
    $$PWD/include/ogdf/planarity/boyer_myrvold/BoyerMyrvoldPlanar.h \
    $$PWD/include/ogdf/planarity/boyer_myrvold/FindKuratowskis.h \
    $$PWD/include/ogdf/planarity/embedder/ConnectedSubgraph.h \
    $$PWD/include/ogdf/planarity/embedder/CrossingStructure.h \
    $$PWD/include/ogdf/planarity/embedder/EmbedderBCTreeBase.h \
    $$PWD/include/ogdf/planarity/embedder/EmbedderMaxFaceBiconnectedGraphs.h \
    $$PWD/include/ogdf/planarity/embedder/EmbedderMaxFaceBiconnectedGraphsLayers.h \
    $$PWD/include/ogdf/planarity/embedder/LayersBlockEmbedder.h \
    $$PWD/include/ogdf/planarity/embedder/MDMFLengthAttribute.h \
    $$PWD/include/ogdf/planarity/embedding_inserter/CrossingsBucket.h \
    $$PWD/include/ogdf/planarity/embedding_inserter/FixEdgeInserterCore.h \
    $$PWD/include/ogdf/planarity/embedding_inserter/VarEdgeInserterCore.h \
    $$PWD/include/ogdf/planarity/embedding_inserter/VarEdgeInserterDynCore.h \
    $$PWD/include/ogdf/planarity/planar_subgraph_fast/MaxSequencePQTree.h \
    $$PWD/include/ogdf/planarity/planar_subgraph_fast/PlanarSubgraphPQTree.h \
    $$PWD/include/ogdf/planarity/planar_subgraph_fast/whaInfo.h \
    $$PWD/include/ogdf/planarity/planarization_layout/CliqueReplacer.h \
    $$PWD/include/ogdf/planarity/BoothLueker.h \
    $$PWD/include/ogdf/planarity/BoyerMyrvold.h \
    $$PWD/include/ogdf/planarity/EdgeTypePatterns.h \
    $$PWD/include/ogdf/planarity/EmbedderMaxFace.h \
    $$PWD/include/ogdf/planarity/EmbedderMaxFaceLayers.h \
    $$PWD/include/ogdf/planarity/EmbedderMinDepth.h \
    $$PWD/include/ogdf/planarity/EmbedderMinDepthMaxFace.h \
    $$PWD/include/ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h \
    $$PWD/include/ogdf/planarity/EmbedderMinDepthPiTa.h \
    $$PWD/include/ogdf/planarity/EmbedderOptimalFlexDraw.h \
    $$PWD/include/ogdf/planarity/ExtractKuratowskis.h \
    $$PWD/include/ogdf/planarity/FixedEmbeddingInserter.h \
    $$PWD/include/ogdf/planarity/KuratowskiSubdivision.h \
    $$PWD/include/ogdf/planarity/MaximalPlanarSubgraphSimple.h \
    $$PWD/include/ogdf/planarity/MaximumPlanarSubgraph.h \
    $$PWD/include/ogdf/planarity/MMFixedEmbeddingInserter.h \
    $$PWD/include/ogdf/planarity/MMSubgraphPlanarizer.h \
    $$PWD/include/ogdf/planarity/MMVariableEmbeddingInserter.h \
    $$PWD/include/ogdf/planarity/MultiEdgeApproxInserter.h \
    $$PWD/include/ogdf/planarity/NodeTypePatterns.h \
    $$PWD/include/ogdf/planarity/NonPlanarCore.h \
    $$PWD/include/ogdf/planarity/PlanarizationGridLayout.h \
    $$PWD/include/ogdf/planarity/PlanarizationLayout.h \
    $$PWD/include/ogdf/planarity/PlanarSubgraphBoyerMyrvold.h \
    $$PWD/include/ogdf/planarity/PlanarSubgraphCactus.h \
    $$PWD/include/ogdf/planarity/PlanarSubgraphEmpty.h \
    $$PWD/include/ogdf/planarity/PlanarSubgraphFast.h \
    $$PWD/include/ogdf/planarity/PlanarSubgraphTree.h \
    $$PWD/include/ogdf/planarity/PlanRep.h \
    $$PWD/include/ogdf/planarity/PlanRepExpansion.h \
    $$PWD/include/ogdf/planarity/PlanRepInc.h \
    $$PWD/include/ogdf/planarity/PlanRepLight.h \
    $$PWD/include/ogdf/planarity/RemoveReinsertType.h \
    $$PWD/include/ogdf/planarity/SimpleEmbedder.h \
    $$PWD/include/ogdf/planarity/SimpleIncNodeInserter.h \
    $$PWD/include/ogdf/planarity/SubgraphPlanarizer.h \
    $$PWD/include/ogdf/planarity/VariableEmbeddingInserter.h \
    $$PWD/include/ogdf/planarity/VariableEmbeddingInserterBase.h \
    $$PWD/include/ogdf/planarity/VariableEmbeddingInserterDyn.h \
    $$PWD/include/ogdf/planarlayout/mixed_model_layout/IOPoints.h \
    $$PWD/include/ogdf/planarlayout/mixed_model_layout/MixedModelBase.h \
    $$PWD/include/ogdf/planarlayout/mixed_model_layout/MMOrder.h \
    $$PWD/include/ogdf/planarlayout/BiconnectedShellingOrder.h \
    $$PWD/include/ogdf/planarlayout/BitonicOrdering.h \
    $$PWD/include/ogdf/planarlayout/FPPLayout.h \
    $$PWD/include/ogdf/planarlayout/LeftistOrdering.h \
    $$PWD/include/ogdf/planarlayout/MixedModelLayout.h \
    $$PWD/include/ogdf/planarlayout/MMCBBase.h \
    $$PWD/include/ogdf/planarlayout/MMCBDoubleGrid.h \
    $$PWD/include/ogdf/planarlayout/MMCBLocalStretch.h \
    $$PWD/include/ogdf/planarlayout/PlanarDrawLayout.h \
    $$PWD/include/ogdf/planarlayout/PlanarStraightLayout.h \
    $$PWD/include/ogdf/planarlayout/SchnyderLayout.h \
    $$PWD/include/ogdf/planarlayout/ShellingOrder.h \
    $$PWD/include/ogdf/planarlayout/TriconnectedShellingOrder.h \
    $$PWD/include/ogdf/simultaneous/SimDraw.h \
    $$PWD/include/ogdf/simultaneous/SimDrawCaller.h \
    $$PWD/include/ogdf/simultaneous/SimDrawColorizer.h \
    $$PWD/include/ogdf/simultaneous/SimDrawCreator.h \
    $$PWD/include/ogdf/simultaneous/SimDrawCreatorSimple.h \
    $$PWD/include/ogdf/simultaneous/SimDrawManipulatorModule.h \
    $$PWD/include/ogdf/simultaneous/TwoLayerCrossMinSimDraw.h \
    $$PWD/include/ogdf/tree/LCA.h \
    $$PWD/include/ogdf/tree/RadialTreeLayout.h \
    $$PWD/include/ogdf/tree/TreeLayout.h \
    $$PWD/include/ogdf/uml/FixedEmbeddingInserterUML.h \
    $$PWD/include/ogdf/uml/OrthoLayoutUML.h \
    $$PWD/include/ogdf/uml/PlanarizationLayoutUML.h \
    $$PWD/include/ogdf/uml/PlanRepUML.h \
    $$PWD/include/ogdf/uml/SubgraphPlanarizerUML.h \
    $$PWD/include/ogdf/uml/UMLGraph.h \
    $$PWD/include/ogdf/uml/VariableEmbeddingInserterDynUML.h \
    $$PWD/include/ogdf/uml/VariableEmbeddingInserterUML.h \
    $$PWD/include/ogdf/upward/internal/UpSAT.h \
    $$PWD/include/ogdf/upward/internal/UpwardPlanarityEmbeddedDigraph.h \
    $$PWD/include/ogdf/upward/internal/UpwardPlanaritySingleSource.h \
    $$PWD/include/ogdf/upward/DominanceLayout.h \
    $$PWD/include/ogdf/upward/ExpansionGraph.h \
    $$PWD/include/ogdf/upward/FaceSinkGraph.h \
    $$PWD/include/ogdf/upward/FixedEmbeddingUpwardEdgeInserter.h \
    $$PWD/include/ogdf/upward/FUPSSimple.h \
    $$PWD/include/ogdf/upward/LayerBasedUPRLayout.h \
    $$PWD/include/ogdf/upward/MaximalFUPS.h \
    $$PWD/include/ogdf/upward/SubgraphUpwardPlanarizer.h \
    $$PWD/include/ogdf/upward/UpwardPlanarity.h \
    $$PWD/include/ogdf/upward/UpwardPlanarSubgraphSimple.h \
    $$PWD/include/ogdf/upward/UpwardPlanRep.h \
    $$PWD/include/ogdf/upward/VisibilityLayout.h \
    $$PWD/src/coin/Clp/Clp_C_Interface.h \
    $$PWD/src/coin/Clp/ClpConfig.h \
    $$PWD/src/coin/Clp/config_clp_default.h \
    $$PWD/src/coin/Clp/config_default.h \
    $$PWD/src/coin/Osi/config_default.h \
    $$PWD/src/coin/Osi/config_osi_default.h \
    $$PWD/src/coin/Osi/OsiConfig.h

