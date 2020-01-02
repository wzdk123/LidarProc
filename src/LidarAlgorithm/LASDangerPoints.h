#pragma once
//
// Created by wuwei on 18-1-24.
// email��wuwei_cug@163.com

#ifndef LASGUI_LASDANGERPOINTS_H
#define LASGUI_LASDANGERPOINTS_H

#include <vector>

#include "../LidarGeometry/Geometry.h"
#include"../LidarGeometry/GeometryFlann.h"
#include "../LidarBase/LASPoint.h"
#include "../LidarBase/LASReader.h"
using namespace GeometryLas;

//pre define complex flann type
typedef PointCloudAdaptor<std::vector<Point3D>> PCAdaptor;
typedef KDTreeSingleIndexAdaptor<L2_Simple_Adaptor<double, PCAdaptor>, PCAdaptor, 3> kd_tree;


/**
* Danger points classify
*/
class LASDangerPoints {
public:
	/**
	* detect danger points
	* @param distance
	* @return
	*/
	virtual long LASDangerPoints_Detect(float distance, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

	/**
	* danger level according distance
	* @param distance
	* @param dangerSectionNumber
	* @param datasetLine
	* @param datasetVegterain
	* @return
	*/
	virtual long LASDangerPoints_Detect(float* distance, int dangerSectionNumber, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

private:
	/**
	* split the danger point from dataset
	* @param datasetVegterain
	* @param pathSplit
	* @return
	* deleted by Frank.Wu memory used too much
	long LADDangerPoints_SplitDanger(ILASDataset* datasetVegterain, const char* pathSplit);
	*/

	/**
	* check each point if the point is danger points
	* @param distance
	* @param pnt
	* @param datasetVegterian
	* @return
	*/
	long LASDangerPoints_PerPoint(float distance, const Point3D* pnt, ILASDataset* datasetVegterian);

	/**
	* danger level according distance pre point
	* @param distance
	* @param dangerSectionNumber
	* @param pnt
	* @param datasetVegterian
	* @return
	*/
	long LASDangerPoints_PerPoint(float* distance, int dangerSectionNumber, const Point3D* pnt, ILASDataset* datasetVegterian);

protected:
	/**
	* check point range of the point
	* @param pnt
	* @param distance
	* @param datasetVegterain
	* @param rectIds
	* @return
	*/
	long LASDangerPoints_Range(const Point3D *pnt, float distance, ILASDataset* datasetVegterain, std::vector<int> &rectIds);
};

/*
	��ȡ��·Σ�յ���Ϣ��ͨ��ANN�㷨���м���ʵ�֣�����ڼ򵥷ֿ�?
	����kdtree�㷨�ܹ��������ߴ���Ч��
	***����Adaptor�Լ����������Ĺ�������ڴ����������Ҫ�ϴ��ڴ�***
	***һ����˵��Ҫ�������ƴ�С���������ڴ���ܹ����д��������ܲ����ڴ治�����***
	Ŀǰ�����ã����ø��Ӹ�Ч�Ļ������ģ�?
	Created by Frank.Wu 2018-07-19
*/
class LASDangerPointsFlann:public LASDangerPoints
{
public:

	/**
	* detect danger points from the single las dataset
	* 想的简单，但是具体实现需要考虑是否重新集成回原来的
	* 数据集中，如果集成回去应该如何处理？
	* param distance: distance thresthold
	* param dataset: dataset las dataset
	* return 
	**/
	virtual long LASDangerPoints_Detect(float distance,ILASDataset* dataset);

	/**
	* danger level according distance
	* param: distance array
	* param: dangerScetionNum
	**/
	virtual long LASDangerPoints_Detect(float* distance, int dangerSectionNumber, ILASDataset* dataset);

	/**
	* ���ݾ����ȡ��·Σ�յ����?
	* @param distance
	* @return
	*/
	virtual long LASDangerPoints_Detect(float distance, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

	/**
	* danger level according distance
	* @param distance
	* @param dangerSectionNumber
	* @param datasetLine
	* @param datasetVegterain
	* @return
	*/
	virtual long LASDangerPoints_Detect(float* distance, int dangerSectionNumber, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

private:
	/**
	* 判断每一个点是不是危险点
	* distance : distance
	* pnt :  line point
	* treeVege : vegetation point kdtree
	**/
	long LASDangerPoints_PerPoint(float distance, const Point3D* pnt,kd_tree &treeVege,vector<int> mapper, ILASDataset* dataset);

	/**
	* ���ĳһ���㷶�?
	* @param distance
	* @param pnt
	* @param treeVege
	* @return
	*/
	long LASDangerPoints_PerPoint(float distance, const Point3D* pnt, kd_tree &treeVege, ILASDataset* datasetVegterain);

	/**
	* danger level according distance pre point
	* @param distance
	* @param dangerSectionNumber
	* @param pnt
	* @param datasetVegterian
	* @return
	*/
	long LASDangerPoints_PerPoint(float* distance, int dangerSectionNumber, const Point3D* pnt, kd_tree &treeVege, ILASDataset* datasetVegterain);
};

/*
	��ȡ��·Σ�յ���Ϣ��ͨ��Block ANN�㷨����ʵ�֣�
	��ǰ���㷨�����⹹��kdtrdd��Adaptor���ڹ����?
	��������Ҫ���¿����ڴ棬�൱�ڽ����ݶ����ڴ�����
	��˻ز������ߵ��ڴ����ģ��Ľ�����㷨��ÿһ������
	����kdtree��Ȼ��ֿ���бȽϺͷ���������˴���Ч��?
	�������ڴ����ģ�ֻ�������ݶ�ȡ�����й������ȽϺ�ʱ
	Created by Frank.Wu 2018-08-31
*/
class LASDangerPointsFlannBlock :public LASDangerPoints
{
public:
	/**
	* ���ݾ����ȡ��·Σ�յ����?
	* @param distance ����Ҫ��
	* @return ���ش������?
	*/
	virtual long LASDangerPoints_Detect(float distance, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

	/**
	* danger level according distance
	* @param distance
	* @param dangerSectionNumber
	* @param datasetLine
	* @param datasetVegterain
	* @return
	*/
	virtual long LASDangerPoints_Detect(float* distance, int dangerSectionNumber, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

	/*
	* get danger points from the inner classify
	* @param distance
	* @param dangerSectionNumber
	* @param classified dataset
	*/
	virtual long LASDangerPoints_Detect(float* distance, int dangerSectionNumber, ILASDataset* dataset,double range,Point2D* ptTower, std::vector<LASIndex> &pntIdx);
private:
	/**
	* 危险点提取单个点处理
	* @param distance
	* @param pnt
	* @param treeVege
	* @return
	*/
	long LASDangerPoints_PerPoint(float distance, const Point3D* pnt,ILASDataset* datasetVegterain);

	/**
	* danger level according distance pre point
	* @param distance
	* @param dangerSectionNumber
	* @param pnt
	* @param datasetVegterian
	* @return
	*/
	long LASDangerPoints_PerPoint(float* distance, int dangerSectionNumber, const Point3D* pnt, ILASDataset* datasetVegterain);

	/**
	* danger level according distance pre point
	* @param distance
	* @param dangerSectionNumber
	* @param pnt
	* @param treeVege:kdtree of the dataset
	* @param treeIdx:las index
	* @param dataset:dataset
	* @return
	*/
	long LASDangerPoints_PerPoint(float* distance, int dangerSectionNumber, const Point3D pnt,
		kd_tree &treeLine, const LASIndex treeIdx, ILASDataset* dataset);
};
/*
* detect falling trees
* */
class LASFallingTreesDangerPoints : public LASDangerPoints
{
public:
	/**
	* get height at the placce
	* @param dataImg: dem data
	* @param xsize :xsize of the dem image
	* @param ysize :ysize of the dem image
	* @param adfGeoTrans :geo transform of the dem image
	* @param gx :geo position x
	* @param gy :geo posision y
	* @return height
	*/
	float LASDangerPoints_Elevation(float* dataImg, int xsize, int ysize, double* adfGeoTrans, float gx, float gy);

	/**
	*
	* @param distance
	* @param dangerSectionNumber
	* @param pathDEM
	* @param datasetLine
	* @param datasetVegterain
	* @return error code
	*/
	long LASDangerPoints_FallingTree(float* distance, int dangerSectionNumber, const char* pathDEM, ILASDataset* datasetLine, ILASDataset* datasetVegterain);

private:
	/**
	*
	* @param distance
	* @param dangerSectionNumber
	* @param demData
	* @param xsize
	* @param ysize
	* @param pnt
	* @param datasetVegterian
	* @return
	*/
	long LASDangerPoints_FllingTree_PrePoint(float* distance, int dangerSectionNumber, float *demData, int xsize, int ysize,
		double *adfGeotrans, const Point3D* pnt, ILASDataset* datasetVegterian);
};


/*
	merge danger point datset to discrete points
	using dbscan algorithm to process
	get danger points and set classify discret distance
*/
class LASDangerPointsMergeArrgegate
{
public:
	/*
		extract danger points from lastadaset
		@param:asDataset:las dataset;
		@param:dangerPnts:danger points extract from dataset
	*/
	long LASDangerExtract(ILASDataset* lasDataset, Point3Ds &dangerPnts);

	/*
		extract correspond line point with the danger points
		@param:lineDataset line point set;
		@param:dangerPnts danger points;
		@linePnts:correspond line points
	*/
	long LASDangerExtractLinePoints(ILASDataset *lineDataset, Point3Ds dangerPnts, Point3Ds &linePnts);
	
	/*
		aggregate points set as a distance using knn
		@param:dangerPnts danger point set;
		@param:type aggregate type(directly using nature number)
		@param:knnRange range distance
	*/
	long LASDangerAggregate(Point3Ds dangerPnts,int *type,float knnRange);

	/*
		merge one type to one point and find the correspond line points
		@param dangerPnts:danger points
		@param type:class types
		@param typeNumbers:type numbers
		@pntDiscrete:point with the correspond line pts
		@correspond:correspond danger points and line point 
	*/
	long LASDangerMerge(Point3Ds dangerPnts, int *type, int typeNumbers, Point3Ds linePnts, int *correspondPairs);

	//for test
	long LASDangerTestFlann();
};

#endif //LASGUI_LASDANGERPOINTS_H
